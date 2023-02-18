# Weak and Strong

```rust
use std::rc::Rc;
use std::rc::Weak;
use std::cell::RefCell;

struct Owner {
    name: String,
    gadgets: RefCell<Vec<Weak<Gadget>>>,
}

struct Gadget {
    id: i32,
    owner: Rc<Owner>,
}

fn main() {
    let gadget_owner: Rc<Owner> = Rc::new(
    	Owner {
            name: "Gadget Man".to_string(),
            gadgets: RefCell::new(Vec::new()),
        }
    );
    
    let gadget1 = Rc::new(Gadget{id: 1, owner: gadget_owner.clone()});
    let gadget2 = Rc::new(Gadget{id: 2, owner: gadget_owner.clone()});
    
    gadget_owner.gadgets.borrow_mut().push(Rc::downgrade(&gadget1));
    gadget_owner.gadgets.borrow_mut().push(Rc::downgrade(&gadget2));
    
    for gadget_opt in gadget_owner.gadgets.borrow().iter() {
        let gadget = gadget_opt.upgrade().unwrap();
        println!("Gadget {} owned by {}", gadget.id, gadget.owner.name);
    }
}
```

​	这里存在双向的引用，可能存在环路，所以可能存在`Rc`无法归零的情况，所以将一个方向的引用设置为`Weak`，压入的时候需要进行`downgrade`。