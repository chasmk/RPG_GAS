### 如何实现鼠标在物体上时高亮显示

- 通过接口 interface！(作为抽象类 声明纯虚函数)
  - enemy类继承这个接口，然后实现高亮函数
- 如何找到鼠标下的敌人
  - Controller里的`GetHitResultUnderCursorByChannel`方法
    - 先`DeProject`鼠标位置
    - 再做`line trace`，返回`HitRresult`
    - 注意enemy的channel要设置为block
- 如何实现高亮？
  - 通过后处理
  - 设置 custom depth stencil value = 特定值（250）

### 接口的使用

比如characterBase类继承一个接口，那么角色类和敌人类都能共享这个接口里的方法

- 例子
  - 角色和敌人获取level：角色从playerstate里获取，敌人从自己类里获取

### 单例模式在项目中的应用

- `FAuraGameplayTags` 结构体类，以单例形式存在，通过它我们可以直接访问各个Attribute的tag
  - 应用
    - Aura AS类需要遍历所有tag，直接获取该单例即可方便地得到所有Tag

- `UAuraAssetManager`类。用于初始化上面的结构体类，向`UGameplayTagsManager`添加native的tag

### DataAsset应用

- 创建`UDataAsset`的子类`UAttributeInfo`。里面存放`Array<FAuraAttributeInfo>`
- 实现一个函数，`FindAttributeInfoByTag`，根据tag寻找`attr info`，
- 然后在蓝图里配置好，这样就能在蓝图和c++里很方便地根据tag获取整个attribute的信息了。

- 应用：
  - `WidgetController`的delegate传递`Attr Info`信息到UI

### WidgetController如何把一堆属性值同步到UI

- 在Aura AS中存放一个map，key是tag，value是获取attr的指针。这样不管是广播初始值还是在delegate上绑定lambda函数。都只要遍历这个map即可得到所有信息。

### UBlueprintFunctionLibrary应用

- 创建该类的子类，这样我们就能在蓝图里加入一个函数库，很方便地调用

- 里面的函数都是static类型，所有实例共享
- 例如根据当前随意一个uobject，就能获取playercontroller，进而获取hud，然后获取其widget controller。
- 这里还学习到world，world context等等关系，见[《InsideUE4》GamePlay架构（三）WorldContext，GameInstance，Engine](https://zhuanlan.zhihu.com/p/23167068)
- 

### 鼠标点击移动的实现

- UE topdown 模板的实现：
  - 单次点击使用`SimpleMoveToLocation` ，这个只有在server上ai控制起作用，有自动寻路功能
  - 持续按下使用`AddMovementInput`可以在联机下同步，需要常量输入
- 我们的项目中有联机功能
  - 必须使用`AddMovementInput`
  - 每一帧都需要一个direction
- 根据点击释放时的累计时间判断是点按还是长按
- 用nav库获得到目标点的路径（源码里最终通过函数指针找到findpath函数，里面使用A*算法）[知乎文章生成导航多边形网格](https://zhuanlan.zhihu.com/p/74537236) ，[知乎文章 Detour寻路 A\*](https://zhuanlan.zhihu.com/p/78873379),    [官方寻路文档](https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/ArtificialIntelligence/NavigationSystem/)
- 然后把路径上的点加到spline中生成曲线
- 每一帧都从spline中获取最近的切线direction，然后addmovement（注意在项目设置里把client的nav也打开）
- 每次获取路径后要更新目标点为路径的最后一个点，因为有的目标点不可达，会出错