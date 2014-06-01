/* file bonjourEnC.c */
#include <jni.h>
#include "CditBonjour.h"
#include <stdio.h>

JNIEXPORT void JNICALL
Java_CditBonjour_ditBonjour(JNIEnv *env, jobject obj)
{
  printf("Bonjour tout le monde!\n");
  return;
}
