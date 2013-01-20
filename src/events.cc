// Copyright 2011 Mariano Iglesias <mgiglesias@gmail.com>
#include "./events.h"

EventEmitter::EventEmitter() : node::ObjectWrap() {
}

void EventEmitter::Init() {
}

bool EventEmitter::Emit(const char* event, int argc,
  v8::Handle<v8::Value> argv[]) {
  v8::HandleScope scope;

  int new_argc = argc + 1;
  v8::Handle<v8::Value>* new_argv = new v8::Handle<v8::Value>[new_argc];
  if (new_argv == NULL) {
    return false;
  }

  new_argv[0] = v8::String::New(event);
  for (int i = 0; i < argc; i++) {
    new_argv[i + 1] = argv[i];
  }

  v8::Local<v8::String> emit_symbol = v8::String::NewSymbol("emit");
  v8::Local<v8::Value> emit_v = this->handle_->Get(emit_symbol);
  v8::Local<v8::Function> emit = v8::Local<v8::Function>::Cast(emit_v);
  emit->Call(this->handle_, new_argc, new_argv);

  delete [] new_argv;

  return true;
}
