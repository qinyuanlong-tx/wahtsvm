//
// Created by 覃远龙 on 2018/10/13.
//

#ifndef WHATSVM_METHODTRACE_H
#define WHATSVM_METHODTRACE_H

#include <jvmti.h>

class MethodTrace {

public:
    MethodTrace();
    ~MethodTrace();

    void Init(JavaVM* vm);
    void ParseOptions(const char* str);
    void AddCapability();
    void RegisterEvent();
    static void JNICALL HandleMethodEntry(jvmtiEnv* jvmti, JNIEnv* jni, jthread thread, jmethodID method);


private:
    static jvmtiEnv *m_Jvmti;
    static jrawMonitorID monitor_lock;

};


#endif //WHATSVM_METHODTRACE_H
