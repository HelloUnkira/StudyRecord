#ifndef VARIATION_FUNCTION
#define VARIATION_FUNCTION

////////////////////////////////////////////////////////
//非结构中,简单变参函数/////////////////////////////////
////////////////////////////////////////////////////////
#define arg_list char*
#define arg_list_start(list, last_param) \
    (list = (arg_list)&last_param + sizeof(last_param))
#define arg_list_get(list, type) \
    ((type*)(list += sizeof(type)))[-1];
#define arg_list_end(list) (list = (arg_list)0)

static argument_parse(int argc, ...)
{
    char* argument_list = NULL;
    argument_list = (char*)&argc + sizeof(argc);
    for (int i =0; i < argc; i++) {
        int value = ((int*)(argument_list += sizeof(int)))[-1];
    }
    argument_list = NULL;
}
////////////////////////////////////////////////////////
//变参//////////////////////////////////////////////////
////////////////////////////////////////////////////////

#endif
