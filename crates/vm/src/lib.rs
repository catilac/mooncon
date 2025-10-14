const MEMORY_SIZE: usize = 2 << 14;
const _STACK: usize =  2 << 12;
const _VRAM: usize =  2 << 13;
const _DISPLAY_SIZE: usize = 2048;

pub struct VM {
    i: u16,
    reg: [i8; 16],
    pc: u16,
    sp: u8,
    mem: [u16; MEMORY_SIZE],
    is_halt: bool
}

impl VM {

    pub fn init() -> Self {
        VM {
            i: 0,
            reg: [0;16],
            pc: 0,
            sp: 0xFF,
            mem: [0; MEMORY_SIZE],
            is_halt: false,
        }
    }

}

// #[cfg(test)]
// mod tests {
//     use super::*;
// 
//     #[test]
//     fn it_works() {
//         let result = add(2, 2);
//         assert_eq!(result, 4);
//     }
// }
