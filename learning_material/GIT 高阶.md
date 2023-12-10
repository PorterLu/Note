## GIT 高阶

### git cherry-pick

​	给定一个或者多个已有的`commit`，应用每一个`commit`引入的更改，并且为每一个commit记录一个新的commit。

![](https://raw.githubusercontent.com/Porterlu/picgo/main/git_cherry_pick_example1.png)

​	使用如下的命令：

```shell
git checkout master
git cherry-pick commit5
```

![](https://raw.githubusercontent.com/Porterlu/picgo/main/git_cherry_pick_example2.png)

​	这样就可以将其他分支上的`commit`引入到`master`，将其作为`master`的`commit`。

### git rebase

​	`rebase`是一种将一系列的`commit`移动或者合并到一个新的`base commit`，

![](https://raw.githubusercontent.com/Porterlu/picgo/main/git_rebase_example.png)

​	`rebase`是为了维护一个线性的`project history`。

### git squash

如何如下的命令：

```shell
git rebase -i <base>
```

​	该命令会产生一个编辑窗口，选择要操作的`commit`历史，可以一条或者多条历史标记为`squash`，那么他们将会和上面一条`commit`相合并。

### git merge

```shell
git merge --squash <branch>
```

​	与普通的`merge`自动创建一个`merge commit`不同，`--squash`会自动在`working copy`中留下一个`local change`，然后之后我们可以自己进行提交。

### git stash
  这次对stash命令进行系统的总结
* git stash save "save message"， 可以将修改的操作存到暂存的缓冲区，而不用进行提交
* git stash list，可以列出现在所有暂存的修改
* git stash pop，应用缓存的修改，并在列表中将对应的缓存进行删除
* git stash apply，和pop类似，但是不进行删除的操作
* git stash drop，删除某一缓存
