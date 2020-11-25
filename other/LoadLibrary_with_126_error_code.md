# LoadLibrary加载dll失败

对于dll加载失败的问题通常可以使用两种方式解决：

1. 使用dependency查看工具，查看有没有dll依赖异常的情况，工具链接见：https://github.com/lucasg/Dependencies
2. 但是通过dependencies查看的时候，可能出现看上去一切都ok，但是应用一调用LoadLibrary就会返回失败，那么这个时候可以借用**Process Monitor** http://technet.microsoft.com/en-us/sysinternals/bb896645 ，将过滤规则设置成待观察的进程，查看该进程在对哪些文件进行访问，从文件的访问排查具体原因。可能会是，加载到了期望之外的别的path下面的同名的dll。

