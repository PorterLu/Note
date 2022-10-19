# 公钥配置  
1.配置ssh key，现在ssh-1已经无法使用，可以使用ssh-kengen -t ed25519 -C "email@**.com",将生成的公钥复制到github中setting的公钥中.  
2.之后使用ssh -T git@github.com进行验证.  
# 配置基本信息  
1.git init进行初始化  
2.git config --global user.email "email@**.com"和git config --global user.name "name"进行配置  
3.git remote add origin(等等) git@github.com:yourName/yourRepo.git添加远程仓库  
# 使用github  
1.可以git clone 路径创建一个克隆版本  
2.git add * 可以添加到缓存区  
3.git commit -m "\*\*\*" 可以将缓存中的内容提交到版本库, 使用git commit -am 可以合并add和commit  
4.git push origin master(或者其他分支) 将该分支提交到远程  
5.git branch 名字 可以创建一个分支，而git checkout 分支名可以切换一个分支  
6.git pull 可以将远程的内容同步到当前分支是fetch和merge的一步化操作，merge有冲突需要再修改后用git add .通知已经修改冲突处，用git commit完成冲突merge  
7.git reset,用于git的回退，如git reset --soft HEAD^,这样就撤销了1次commit，但是代码任然保留，HEAD^的意思是上一个版本，也可以写成HEAD\~1,如果想进行了两次commit都想撤回，可以使用HEAD\~2。使用--mixed选项也意味着不删除工作空间改动代码，而是撤销commit，并且git add。所以git reset --mixed HEAD^的作用和git reset HEAD^的效果是一样的。git --hard相较于git --soft会删除工作空间改动代码。 
8.git submodule add git_addr,可以添加一个submodule，可以用于管理子项目，进入子项目目录，执行如下指令:
```
git submodule init
git submodule update
```
  
