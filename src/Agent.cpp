//
// Created by 覃远龙 on 2018/10/12.
//

#include <iostream>
#include "Agent.h"
#include "MethodTrace.h"

using namespace std;

static jvmtiEnv *globalJVMTIInterface = 0;

JNIEXPORT jint JNICALL
Agent_OnAttach(JavaVM *jvm, char *options, void *reserved)
{

}

void JNICALL
MethodEntryCallback(jvmtiEnv *jvmti,
                                 JNIEnv *jni,
                                 jthread thread,
                                 jmethodID method)
{
    //string methodName = get_method_name(*jvmti, method);
    //std::cout << boost::format("Enter Method: %s\n") % methodName;
}

JNIEXPORT void
JNICALL Agent_OnUnload(JavaVM *vm)
{}