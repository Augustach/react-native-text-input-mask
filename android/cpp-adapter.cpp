
#include <iostream>
#include <jsi/jsi.h>
#include <jni.h>

// global ref to Java Virtual Machine
// used to get JNI current env
JavaVM *jvm;
// global ref to our class instance
static jobject jthisRef;

using namespace facebook;
//using namespace facebook::jni;

JNIEnv *attachCurrentThread()
{
  JavaVMAttachArgs args{JNI_VERSION_1_6, nullptr, nullptr};
  JNIEnv *env = nullptr;
  auto result = jvm->AttachCurrentThread(&env, &args);
  return env;
}

void install(jsi::Runtime& jsiRuntime, jobject jthis) {
    // rntextinputmaskMask
    auto rntextinputmaskMask = jsi::Function::createFromHostFunction(jsiRuntime,
                                                         facebook::jsi::PropNameID::forAscii(jsiRuntime, "rntextinputmaskMask"),
                                                         3,  // maskString, inputValue, autocomplete
                                                         [](facebook::jsi::Runtime& runtime, const facebook::jsi::Value& thisValue, const facebook::jsi::Value* arguments, size_t count) -> jsi::Value {
        auto env = attachCurrentThread();
        jclass thisClass = env->GetObjectClass(jthisRef);
        jmethodID unmaskValue = env->GetMethodID(thisClass, "mask", "(Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;");
        jstring maskString = env->NewStringUTF(arguments[0].getString(runtime).utf8(runtime).c_str());
        jstring inputValue = env->NewStringUTF(arguments[1].getString(runtime).utf8(runtime).c_str());
        jboolean autocomplete = arguments[2].getBool();
        auto unmaskJString = (jstring)env->CallObjectMethod(jthisRef, unmaskValue, maskString, inputValue, autocomplete);
        const char *unmaskChars = env->GetStringUTFChars(unmaskJString, nullptr);
        return jsi::String::createFromAscii(runtime, unmaskChars);
    });
    jsiRuntime.global().setProperty(jsiRuntime, "rntextinputmaskMask", std::move(rntextinputmaskMask));


    // rntextinputmaskUnmask
    auto env = attachCurrentThread();
    jclass thisClass = env->GetObjectClass(jthis);
    auto rntextinputmaskUnmask = jsi::Function::createFromHostFunction(jsiRuntime,
                                                                jsi::PropNameID::forAscii(jsiRuntime, "rntextinputmaskUnmask"),
                                                                3,  // maskString, inputValue, autocomplete
                                                                [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {                                                          
        auto env = attachCurrentThread();
        jclass thisClass = env->GetObjectClass(jthisRef);
        jmethodID unmaskValue = env->GetMethodID(thisClass, "unmask", "(Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;");
        jstring maskString = env->NewStringUTF(arguments[0].getString(runtime).utf8(runtime).c_str());
        jstring inputValue = env->NewStringUTF(arguments[1].getString(runtime).utf8(runtime).c_str());
        jboolean autocomplete = arguments[2].getBool();
        auto unmaskJString = (jstring)env->CallObjectMethod(jthisRef, unmaskValue, maskString, inputValue, autocomplete);
        const char *unmaskChars = env->GetStringUTFChars(unmaskJString, nullptr);
        return jsi::String::createFromAscii(runtime, unmaskChars);
    });
    jsiRuntime.global().setProperty(jsiRuntime, "rntextinputmaskUnmask", std::move(rntextinputmaskUnmask));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_RNTextInputMask_RNTextInputMaskModule_nativeInstall(JNIEnv *env, jobject jthis, jlong jsiPtr) {
    // storring java vm reference
    env->GetJavaVM(&jvm);
    jthisRef = env->NewGlobalRef(jthis);
    auto runtime = reinterpret_cast<jsi::Runtime*>(jsiPtr);
    if (runtime) {
        install(*runtime, jthis);
    }
}