# EventManager
Low-coupling event management plugin.

低耦合事件管理插件，应用了蓝图泛型，蓝图使用体验友好。

Note:不要在接口ReceiveMessage函数中调用UnregistryObject。因为在遍历过程中。

注册对象不会影响GC，不取消注册会导致悬空指针。但是会在下次尝试交互时从列表中删除。所以手动调用UnRegistryObject不是必须的。
