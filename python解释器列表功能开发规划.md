## python解释器列表功能开发规划

### 一、列表功能定义

1. 按下标访问[]
2. 添加以及拼接 a + b
3. 删除第一个匹配的元素  a - b
4. 修改a[0] = 3

### 二、任务安排

#### 1. 实现列表基础部分

1. 设计列表抽象语法树（0.5day）

   定义：

   ```php
   def a = [1, 2 + 3];
   def b = [[1, 2], 3];
   ```

   ```mermaid
   graph TB
   a((def ))--- b((a))
   b((a)) --- c((list))
   c((list)) ---d((1))
   c((list)) --- e((+))
   e((+)) --- f((2))
   e((+)) --- g((3))
   ba((def)) --- bb((b))
   bb((b)) --- bc((list))
   bc((list)) --- bd((list))
   bd((list)) --- be((1))
   bd((list)) --- bf((2))
   bc((list)) --- bg((3))
   ```

   赋值：

   ```php
   a = [2, 3]
   ```

   

   ```mermaid
   graph TB
   pz((=)) --- py((a))
   pz((=)) --- pa((list))
   pa((list)) --- pb((2))
   pa((list)) --- pc((3))
   ```

   

   引用：

   ```php
   a[0];
   b[0][0];
   ```

   ```mermaid
   graph TB
   a((list_ind)) --- b((a))
   a((list_ind)) --- c((0))
   d((list_ind)) --- e((b))
   d((list_ind)) --- f((0))
   d((list_ind)) --- g((0))
   ```

2. 修改文法文件hl.g

   ```python
   //定义
   
   //索引
   
   ```
   
   
   
3. 设计实现列表类listValue【haizei_type】(1day)

   1. 函数
      1. size()
      2. convert()
      3. operator+
      4. operator-
   2. 属性
      1. map<int, vector<IValue> > list;
      2. __size

4. 访问者实现 + - ，工厂实现

5. 列表节点的处理ExprMaster::run() 【haizei_master】 （1day）

6. 打印列表的实现

#### 2.测试，修改bug
