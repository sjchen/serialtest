# serialtest
Android and Linux Serial Test tool in C

Compile framework credits: Zhang Yong @BD
编译：
1) android端
    安装并配置android NDK开发工具链，然后在代码目录中执行：make android
2) IDEN端
    把代码上传到IDEN服务器上，执行：
        cleartool setview <YourViewName>
        make iden
3) Ubuntu上测试代码正确性：
    make pc
