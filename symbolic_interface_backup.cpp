#include <node.h>
#include <node_buffer.h>

#include <stddef.h>
extern "C"
{
#include <crete/custom_instr.h>
}
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <vector>

using namespace v8;

// Stays in scope the entire time the addon is loaded.
Persistent<Object> persist;

void MakeConcolicVar(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  Local<Object> target = Local<Object>::New(isolate, persist);

  Local<String> key = String::NewFromUtf8(isolate, "sym_str");
  // pull the current value of prop sym_str out of the object
  Local<String> current = target->ToObject()->Get(key)->ToString();
  String::Utf8Value value(current);
  const char *arg0_str_p = *value;

  char *arg0_str_p_final = new char[strlen(arg0_str_p) + 1];
  memset(arg0_str_p_final, 0, strlen(arg0_str_p) + 1);

  //print out str
  printf("1 buffer. symbolic string is %s, n address is %p not final\n", arg0_str_p, arg0_str_p);
  //printf("2. symbolic string is %s, n address is %p\n", arg0_str_p_final, arg0_str_p_final);

  crete_make_concolic(arg0_str_p_final, sizeof(arg0_str_p) + 1, "nodejs_api_string");

  //return changed address or string back to js
  Local<Object> return_buf;
  if (node::Buffer::New(isolate, arg0_str_p_final, strlen(arg0_str_p) + 1).ToLocal(&return_buf))
  {
    args.GetReturnValue().Set(return_buf);
  }
}
void MakeConcolicVarArray(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  std::vector<std::string> arrayfromjs;
  //Local<Object> target = Local<Object>::New(isolate, persist);

  Local<Array> input_arr = Local<Array>::Cast(args[0]);
  unsigned int num_arr = input_arr->Length();

  printf("0 local array address is %p final\n", &input_arr);

  size_t offset = 0;
  for (unsigned int i = 0; i < num_arr; i++)
  {

    //my code
    Local<Object> item = Local<Object>::Cast(input_arr->Get(i));
    //persist.Reset(isolate, item);
    Local<String> item_str = item->ToString();

    String::Utf8Value value(item_str);
    const char *arg0_str_p = *value;
    char *arg0_str_final = const_cast<char *>(arg0_str_p);
    char *bytePtr = reinterpret_cast<char *>(arg0_str_final);
    bytePtr += offset;
    
    //debug info
    printf("1 array item. symbolic string is %s, n array item address is %p final\n", arg0_str_final, arg0_str_final);
    printf("2 array item. symbolic string is %s, n array item address is %p final\n", arg0_str_final, bytePtr);
    printf("3 array item. symbolic string is %s, n array item address is %p final\n", item_str, &item_str);
    printf("4 array item address is %p final\n", &item);
    //debug info

    //crete_make_concolic(arg0_str_final, sizeof(arg0_str_final), "nodejs_api_array");
    offset += sizeof(arg0_str_final);

    std::string str(arg0_str_final);
    arrayfromjs.push_back(arg0_str_final);
  }

  Local<Array> result_list = Array::New(isolate);
  for (unsigned int i = 0; i < arrayfromjs.size(); i++)
  {
    //Local<Object> resitem = Object::New(isolate);
    printf("arrayfromjs item is %s, address is %p\n", arrayfromjs[i].c_str(), &arrayfromjs[i]);
    
    char *item_c = const_cast<char *>(arrayfromjs[i].c_str());

    printf("arrayfromjs item_c is %s, address is %p\n", item_c, item_c);

    Local<String> resitem = String::NewFromUtf8(isolate, item_c);
    result_list->Set(i, resitem);
  }

  args.GetReturnValue().Set(result_list);

  //testing
  /**
  if(strcmp(arg0_str_p_final, "symbolicper") == 0 ){
    printf("symbolic persist first branch\n");
  }else{
    printf("symbolic persist seconde branch\n");
  }
  **/
}

void StartAnalysis(const FunctionCallbackInfo<Value> &args)
{

  printf("enter analysis\n");
  crete_send_target_pid();
  printf("leaving analysis\n");
}

void EndAnalysis(const FunctionCallbackInfo<Value> &args)
{
  printf("enter exit analysis\n");
  exit(0);
}

void Setup(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  // Save a persistent handle to this object for later use in Mutate
  persist.Reset(isolate, args[0]->ToObject());
}

void init(Local<Object> exports)
{
  NODE_SET_METHOD(exports, "setup", Setup);
  NODE_SET_METHOD(exports, "makeConcolicVar", MakeConcolicVar);
  NODE_SET_METHOD(exports, "makeConcolicVarArray", MakeConcolicVarArray);
  NODE_SET_METHOD(exports, "startAnalysis", StartAnalysis);
  NODE_SET_METHOD(exports, "endAnalysis", EndAnalysis);
}

NODE_MODULE(symbolic_interface, init)
