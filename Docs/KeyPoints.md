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