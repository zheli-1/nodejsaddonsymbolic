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
Persistent<String> persist_string;

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

  // crete_make_concolic(arg0_str_p_final, sizeof(arg0_str_p) + 1, "nodejs_api_string");

  //return changed address or string back to js
  Local<Object> return_buf;
  if (node::Buffer::New(isolate, arg0_str_p_final, strlen(arg0_str_p) + 1).ToLocal(&return_buf))
  {
    args.GetReturnValue().Set(return_buf);
  }
}

void MarkSymbolicVar(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  //Local<Object> target = Local<Object>::New(isolate, persist);


  Local<String> current =args[0]->ToString();


  // pull the current value of prop sym_str out of the object
  String::Utf8Value value(current);
  const char *arg0_str_p = *value;
  std::string sym_key="symkey";
  std::string str(arg0_str_p);
  printf("arg0_str_p is %s\n", arg0_str_p);
  //str.insert(strlen(arg0_str_p), sym_key);
  
  int n = str.length();
  char char_array[n+1];
  strcpy(char_array, str.c_str());

  //crete_make_concolic(char_array, n+1, "nodejs_api_string");

  
  printf("str is %s, length is %d\n", char_array, n);
  Local<String> local_str = String::NewFromUtf8(isolate, char_array);
  printf("local_str address is %p\n", &local_str);
  args.GetReturnValue().Set(local_str);
}

void MarkSymbolicObject (const FunctionCallbackInfo<Value> &args)
{
  //Isolate *isolate = args.GetIsolate();
  //Local<Object> target = Local<Object>::New(isolate, persist);
  Local<Object> current =args[0];

  //crete_make_concolic(args[0], n+1, "nodejs_api_string");


  args.GetReturnValue().Set(current);
}

void MakeConcolicVarArray(const FunctionCallbackInfo<Value> &args)
{
  printf("args stack address is %p, address is %p\n", (void*)&args, args);
  
  Isolate *isolate = args.GetIsolate();
  printf("isolate stack address is %p, address is %p\n", (void*)&isolate, isolate);
  
  std::vector<std::string> arrayfromjs;
  //Local<Object> target = Local<Object>::New(isolate, persist);

  Local<Array> input_arr = Local<Array>::Cast(args[0]);
  unsigned int num_arr = input_arr->Length();

  printf("input_arr stack address is %p, address is %p\n", (void*)&input_arr, input_arr);

  int64_t offset = 0;
  for (unsigned int i = 0; i < num_arr; i++)
  {
    printf("*****************************\n");
    //my code
    Local<Object> item = Local<Object>::Cast(input_arr->Get(i));
    printf("item stack address is %p, address is %p\n", (void*)&item, item);

    //persist.Reset(isolate, item);
    Local<String> item_str = item->ToString();

    String::Utf8Value value(item_str);
    
    printf("item_str stack address is %p, address is %p\n", (void*)&item_str, item_str);

    const char *arg0_str_p = *value;
    char *arg0_str_final = const_cast<char *>(arg0_str_p);
    char *bytePtr = reinterpret_cast<char *>(arg0_str_final);
    //int64_t bytePtr = reinterpret_cast<int64_t>(&item);
    bytePtr += offset;
    
    //debug info
    printf("value is %s, stack address is %p, address is %p\n", arg0_str_p, (void*)&value, arg0_str_p);
    printf("arg0_str_final string is %s, arg0_str_final address is %p\n", arg0_str_final, arg0_str_final);
    printf("bytePtr string is %s, address is %p\n", bytePtr, (void*)&bytePtr);
    //debug info

    //crete_make_concolic(bytePtr, strlen(arg0_str_final), "nodejs_api_array");
    if(i>0){
      Local<Object> item_less = Local<Object>::Cast(input_arr->Get(i-1));
      offset += reinterpret_cast<char*>(&item)-reinterpret_cast<char*>(&item_less);
      char * ptr_item_less = reinterpret_cast<char*>(&item_less);
      printf("------------------------------------\n");
      printf("ptr_item_less string is %s\n", ptr_item_less);
      printf("item address is %p, item_less address is %p\n", item, item_less);
      printf("offset is %d\n", offset);
      printf("------------------------------------\n");      
    }
    std::string str(arg0_str_final);
    arrayfromjs.push_back(arg0_str_final);

    printf("*****************************\n");
  }

  Local<Array> result_list = Array::New(isolate);
  //Local<Object> result_obj = Local<Object>::New(isolate,persist);
  //Local<Array>  result_list = Local<Array>::Cast(result_obj);

  for (unsigned int i = 0; i < arrayfromjs.size(); i++)
  {
    //Local<Object> resitem = Object::New(isolate);
    printf("arrayfromjs[%d] is %s, address is %p\n", i, arrayfromjs[i].c_str(), &arrayfromjs[i]);
    
    char *item_c = const_cast<char *>(arrayfromjs[i].c_str());

    printf("arrayfromjs c_str is %p, item_c address is %p\n", arrayfromjs[i].c_str(), item_c);

    Local<String> resitem = String::NewFromUtf8(isolate, item_c);

    printf("resitem address is %p, %p\n", (void *)&resitem, resitem);
    result_list->Set(i, resitem);
  }

  printf("result_list address is %p\n", (void *)&result_list);

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
  NODE_SET_METHOD(exports, "markSymbolicVar", MarkSymbolicVar);
}

NODE_MODULE(symbolic_interface, init)
