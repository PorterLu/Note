### HDLbits 题目记录
#### Basic
```
1.verilog的代码结构
module top_module(output one);
  assign one = 1;
endmodule

2.wire
wire是一个持续性赋值,输出端的值持续随着输入值变化
module top_module(input in,output out)
  assign out = in;
endmodule

```

#### vector
```
1.wire[7:0] my_vector 可以申请一个向量
vector 指向的可以是一个负数
index 是小->大，还是从大->小，定义和使用的时候请保持一致。

2.assign w = a; 当左右两边都是vector时，则进行0扩展或者缩减
加上default_nettype none 可以解决net问题

3.concatenation连结
{4'b1111,4h'a} 用大括号包起来，并用逗号隔开可以进行连结
{24{3'b111}}可以进行重复连结，用于多个同样的vector的连结
```

#### module层次

```
1.wire连接到port有两种方法
mod_a instance1(wa,wb,wc);这个方法就是按序的连结
mod_a instance2(.out(wc), .in1(wa), .in2(wb)) 这种方法可以不考虑顺序

2.module和module之间的连接就可以用wire进行沟通。
```

#### 过程
```
1.module top_module(
  input clk,
  input [7:0] d,
  input [1:0] sel,
  output[7:0] q
);
  wire[7:0] o1,o2,o3;
  my_dff8 instance1(clk,d,o1);
  my_dff8 instance2(clk,o1,o2);
  my_dff8 instance3(clk,o2,o3);
  
  //multiplexer
  always @(*)
    case(sel)
      2'h0: q = d;
      2'h1: q = o1;
      2'h2: q = o2;
      2'h3: q = o3;
    endcase
endmodule

2.用两个16的全加器实现32位加法，后一个加法需要等待第一个加法的进位结果，但是可以提前算好CF = 0,CF = 1的两种情况，那么
就可以在第一个加法结束时进行一个选择就可以了，从而解决了ripple adder的问题。

3.连结的书写
assign out_assign = a^b;
always @(*) out_always_comb = a^b;
always @(posedge clk) out_always_ff <= a^b;

4.always if的书写

assign out_assign = sel1_b1&sel_b2?b:a;
always @(*) begin
  if(sel1_b1 & sel_b2 == 1) begin
    out_always = b;
  end
  else begin
    out_always = a;
  end
end

5.注意在verilog在你没有说明的情况会保持结果不变，保持结果不变，就意味着要记住当前的状体，而组合逻辑是没法记住状态的，
这种情况下往往会产生bug，记得对于所有的状体赋值。
```

#### 更多的特性
```
1.三元运算符 if a? b:c;
2.reduction 可以逐bit进行操作如  a = &w[7:0] 对0到7共8bits进行余得到1bit的结果
3.for循环
  integer i;
  alway @(*)
  for(i=0;i<100;i++)
    out[i] = in[99-i];
4.generate 用于在一个循环中不断地实例化
genvar i;
generate 
  full_adder start(a[0],b[0],cin,sum[0],cout[0]);
  for(i=0;i<100;i++) begin: adder                //记得取名
    full_adder ins(a[i],b[i],cout[i-1],sum[i],cout[i]);
  end
endgenerate

在generate中赋值记得加上assign
    
```

