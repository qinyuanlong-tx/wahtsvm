//
// Created by 覃远龙 on 2018/10/13.
//

#include <iostream>
#include <stack>

#include "MethodTrace.h"

using namespace std;

void MethodTrace::Init(JavaVM* vm)
{
    jvmtiEnv *jvmti = 0;
    jint ret = (vm)->GetEnv(reinterpret_cast<void**>(&jvmti), JVMTI_VERSION_1_0);

    m_Jvmti = jvmti;

    this->AddCapability();
}

void MethodTrace::ParseOptions(const char *str) {}

void MethodTrace::AddCapability()
{
    // 创建一个新的环境
    jvmtiCapabilities caps;
    memset(&caps, 0, sizeof(caps));
    caps.can_generate_method_entry_events = 1;

    // 设置当前环境
    jvmtiError error = m_Jvmti->AddCapabilities(&caps);
}

void MethodTrace::RegisterEvent()
{
    // 创建一个新的回调函数
    jvmtiEventCallbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.MethodEntry = &MethodTrace::HandleMethodEntry;

    // 设置回调函数
    jvmtiError error;
    error = m_Jvmti->SetEventCallbacks(&callbacks, static_cast<jint>(sizeof(callbacks)));
    //CheckException(error);

    // 开启事件监听
    error = m_Jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, 0);
    //CheckException(error);
}

void JNICALL MethodTrace::HandleMethodEntry(jvmtiEnv* jvmti, JNIEnv* jni, jthread thread, jmethodID methodId) {
    jvmtiError error;
    jclass clazz;
    char *name;
    char *signature;

    // 获得方法对应的类
    error = m_Jvmti->GetMethodDeclaringClass(methodId, &clazz);
    //CheckException(error);
    // 获得类的签名
    error = m_Jvmti->GetClassSignature(clazz, &signature, 0);
    //CheckException(error);
    // 获得方法名字
    error = m_Jvmti->GetMethodName(methodId, &name, NULL, NULL);
    //CheckException(error);

    cout << signature << " -> " << name << "(..)" << endl;

    jint size;
    jvmti->GetArgumentsSize(methodId, &size);
    jint entry_count;
    jvmtiLocalVariableEntry *table;
    jvmtiError jerr = jvmti->GetLocalVariableTable(methodId, &entry_count, &table);

    if (jerr == JVMTI_ERROR_NONE) {
        //cout << "Agent::method_entry- Thread=" << thread_name << " | Method=" << clazz << method << sig << endl;
        for (unsigned int i = 0; i < entry_count; i++) {
            string signature(table[i].signature);
            cout << "\tAgent::method_entry- Variable=" << table[i].name << " Type=" << signature << endl;;
            // Get the content if this is an Object
            jobject value_ptr;
            jvmti->GetLocalObject(thread, (jint) 0, table[i].slot, &value_ptr);
            switch (signature[0]) {
                case 'L' : {
                    // For now, we can only get String objects
                    if (signature.find("java/lang/String") != string::npos) {
                        const char *str = {0};
                        str = jni->GetStringUTFChars((jstring) value_ptr, 0);
                        cout << "\tAgent::method_entry- Content=" << str << endl;
                        jni->ReleaseStringUTFChars((jstring) value_ptr, str);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        jvmti->Deallocate(reinterpret_cast<unsigned char *>(table));

        // 必须释放内存，避免内存泄露
        error = m_Jvmti->Deallocate(reinterpret_cast<unsigned char *>(name));
        //CheckException(error);
        error = m_Jvmti->Deallocate(reinterpret_cast<unsigned char *>(signature));
        //CheckException(error);
    }
}