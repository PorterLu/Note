# Markdown For Typora

## 概述

**Markdown** 由 Daring Fireball 创作。

[toc]

## 块元素

### 段落与行间距

一个段落指的是一到多行连续的文本。在markdown源代码中，段落由两个或者更多的空行隔开。在Typora中，你只需要用一个空行（一个回车）来创建一个段落。  

按下 `Shift` + `Return` 可以创建一个行间距。大多数其它的markdown解释器会忽视行间距，所以为了在其他的markdown中仍然保有行间距，可以在行尾加上两个空格或者插入`<br/>`。   

### 标题

标题使用1\~6个 `#` 在行首来指明1\~6级的标题。例子如下：

```markdown
# 一级标题

## 二级标题

### 三级标题
```

在Typora中，输入 `#` 之后输入标题再按下 `Return` 将创造一个标题。

### 块引用

Markdown使用邮件风格 `>` 符号来进行块引用，它们可以这样书写：

```markdown
> 两个段落的块引用，现在是第一个段落。
> 
> 这是第二个段落。



> 这是另外一个段落的块引用。这里两个块引用间有三个空行。
```

在Typora中，输入 `>` 再输入引用内容就会生成一个引用块。Typora会自动帮你在下一行键入一个 `>` 。嵌套块引用可以通过再键入一级的 `>` 实现。

### 列表

输入 `* list item 1` 会自动创建一个无序的列表——`*` 可以替代为 `+` 或者 `-` 。

输入 `1. list item 1` 会自动创建一个有序的列表，它们的markdown源码如下：

```markdown
## un-ordered list
* Red
* Green
* Blue

## ordered list
1. Red
2. Green
3. Blue
```

###  任务列表

任务列表就是每一个项目用 `[]` 或者 `[x]` 来标记。

例如：

```mark
- [ ] 一个任务列表
- [ ] 列表需要的语法
- [ ] 通常的格式
- [x] 完成的项目
```

你可以通过鼠标点击设置完成/未完成的状态。

### 代码块

Typora支持github风格的markdown代码块，而markdown原始的代码块是不支持的。  

使用代码块很容易：输入 \`\`\`  并按下 `return` 。之后再 \`\`\` 后加上一个额外的语言标识，那么代码块的语法就可以被高亮显示了：

````gfm
这就是例子

```js
function test(){
	console.log("notice the blank line before this function?")
}
```
````

语法高亮:

``` ruby
require 'redcarpet'
markdown = Redcarpet.new("Hello World！")
puts markdown.to_html
```

### 数学块

你可以使用 `**MathJax**` 来渲染 `*LaTex*` 数学表达式。  

为了添加一个数学表达式，输入 `$$` 并按下 `Return` 。就可以触发一个输入框用于输入`*Tex/LaTex*`源代码。例子如下：
$$
\mathbf{V}_1 \times \mathbf{V}_2 = \begin{bmatrix}
\mathbf{i} & \mathbf{j} & \mathbf{k} \\\\
\frac{\partial X}{\partial u} & \frac{\partial Y}{\partial u} & 0 \\\\
\frac{\partial X}{\partial v} & \frac{\partial Y}{\partial v} & 0 \\\\
\end{bmatrix}
$$
在markdown源码文件中，数学块就是用一对 `$$` 包裹的 `LaTex` 表达式：

```markdown
$$
\mathbf{V}_1 \times \mathbf{V}_2 = \begin{vmatrix}
\mathbf{i} & \mathbf{j} & \mathbf{k} \\
\frac{\partial X}{\partial u} & \frac{\partial Y}{\partial u} & 0 \\
\frac{\partial X}{\partial v} & \frac{\partial Y}{\partial v} & 0 \\
\end{vmatrix}
$$
```

### 表格

输入 `| 第一个标题 | 第二个标题 ` 并按下 ` Return ` 键，就可以创建一个有两列的表格。  

在创建完一个表格后，将光标放在表格会出现一个工具栏你可以重设表格大小、设置文字偏移方式或者删除一个表格。你也可以使用菜单的方式去拷贝、添加或者删除一个行/列。  

创建一整个表格的语法如下，但是并没有必要知道完整语法的细节，因为Typora会帮你自动生成markdown源码。

在markdown源码中，它们长这样：  

``` markdown
| First Header  | Second Header |
| ------------- | ------------- |
| Content Cell  | Content Cell  |
| Content Cell  | Content Cell  |
```

你也可以在表格中内联markdown如 链接、加粗、斜体或者中划线。  

最后，通过在标题行添加 `:` ，你可以设定文本在该列是左对齐、右对齐或者居中对齐：

``` markdown
| Left-Aligned  | Center Aligned  | Right Aligned |
| :------------ |:---------------:| -----:|
| col 3 is      | some wordy text | $1600 |
| col 2 is      | centered        |   $12 |
| zebra stripes | are neat        |    $1 |
```

一个冒号在左侧意味着这是一个左对齐的列；一个在右侧的冒号意味着这是一个右对齐的列；两侧都有冒号意味着这是一个居中对齐的列。

### 脚注

```markdown
你可以定义一个脚注，就像这样[^脚注]
[^脚注]：这是一个脚注文本
```

你也可以这样创建可以脚注[^脚注]。  

[^脚注]: 这是一个脚注。

悬停在脚注上可以看到脚注中写的内容。















 
