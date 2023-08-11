[TOC]

## RCU	

​	RCU的意思是Read、Copy、Update，它是一种同步机制，Linux内核中可以用它去优化一些大多数情况只做读的场景。

### 1. 概述

​	RCU最基本的思想就是将更新分为“去除”和“再利用”两个阶段。“去除“意味着可以去除一个数据结构内对于一个变量的引用，可以并发运行读者。因为在这个阶段可以保证读者读到一个要么新的数据要么旧的数据，而不是一个半更新的数据。而“再利用”阶段不能在读者不再持有这些数据前就开始。

​	将更新分为两个阶段就允许更新者可以马上开始“去除”阶段，并且直到读者结束后才开始“再利用”阶段。

a）去除对于一个数据结构的引用，所以读者不可以再获取对于它的引用；

b）所有的读者完成了读的操作；

c）进行“再利用”阶段。

### 2. API and Example

```markdown
rcu_assign_pointer()
                        +--------+
+---------------------->| reader |---------+
|                       +--------+         |
|                           |              |
|                           |              | Protect:
|                           |              | rcu_read_lock()
|                           |              | rcu_read_unlock()
|        rcu_dereference()  |              |
+---------+                 |              |
| updater |<----------------+              |
+---------+                                V
|                                    +-----------+
+----------------------------------->| reclaimer |
                                     +-----------+
  Defer:
  synchronize_rcu() & call_rcu()
```

​	下面是一个例子：

```c
struct foo {
    int a;
    char b;
    long c;
};

DEFINE_SPINLOCK(foo_mutex);

struct foo __rcu *gbl_foo;

/* Create a new struct foo that is the same as the one currently pointed to by gbl_foo, 
 * except that field "a" is replaced with "new_a". Points gbl_foo to the new structure, 
 * and frees up the old structure after a grace period.
 *
 * Uses rcu_assign_pointer() to ensure that concurrent readers see the initialized version
 * of the new structure.
 *
 * Uses synchronize_rcu() to ensure that any readers that might have references to the old
 * structure complete before freeing the old structure.
 */

void foo_update_a(int new_a) {
    struct foo *new_fp;
    struct foo *old_fp;
    
    new_fp = kmalloc(sizeof(*new_fp), GFP_KERNEL);
    spin_lock(&foo_mutex);
    old_fp = rcu_dereference_protected(gbl_foo, lockdep_is_held(&foo_mutex));
    *new_fp = *old_fp;
    new_fp->a = new_a;
    rcu_assign_pointer(gbl_foo, new_fp);
    spin_unlock(&foo_mutex);
    synchronize_rcu();
    kfree(old_fp);
}

/* Return the value of field "a" of the current gbl_foo structure. Use rcu_read_lock() and 
 * rcu_read_unlock() to ensure that the structure does not get deleted out from under us,
 * and use rcu_dereference() to ensure that we see the initialized version of the structu-
 * re.
 */
int foo_get_a(void) {
    int retval;
    
    rcu_read_lock();
    retval = rcu_dereference(gbl_foo)->a;
    rcu_read_unlock();
    return retval;
}
```

​	RCU保证更新数据时，需要复制一份副本，在副本上进行修改，再一次性替换数据。对于被RCU保护的数据，读者不需要任何的锁就可以访问它，但写者在访问时必须先拷贝一个副本，通过对于副本进行修改，最后通过一个回调函数在适当的实际包原来数据的指针指向被修改的数据。

### 3.解决的问题

* 在读取的过程中，另外一个现成删除了一个节点。删除线程可以把这个节点从链表中移除，但它不能直接销毁这个节点，必须等待所有现成读取完成后，才能进行这个销毁操作。RCU将这个过程称之为Grace period。
* 在读取的过程中，另外一个线程插入了一个节点，而读现成读到了这个节点，必须保证这个节点是完整的。