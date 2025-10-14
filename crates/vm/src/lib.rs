use cpu::{Instruction, Op};

const MEMORY_SIZE: usize = 2 << 14;
const _STACK: usize = 2 << 12;
const _VRAM: usize = 2 << 13;
const _DISPLAY_SIZE: usize = 2048;

pub struct VM {
    i: u16,
    reg: [i8; 16],
    pc: u16,
    sp: u8,
    mem: [u16; MEMORY_SIZE],
    is_halt: bool,
}

impl VM {
    pub fn init() -> Self {
        VM {
            i: 0,
            reg: [0; 16],
            pc: 0,
            sp: 0xFF,
            mem: [0; MEMORY_SIZE],
            is_halt: false,
        }
    }

    // TODO
    pub fn fde_cycle(&mut self) {}

    // TODO
    pub fn load_program(&mut self) {}

    fn _fetch(&mut self) -> u16 {
        let m: u16 = self.mem[self.pc as usize];
        self.pc += 1;
        return m;
    }

    fn _decode(_bytecode: u16) -> Instruction {
        return Instruction::init(Op::Nop);
    }

    fn _execute(_i: Instruction) {}
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn initialization() {
        let vm = VM::init();
        assert_eq!(vm.i, 0);
        assert_eq!(vm.reg, [0; 16]);
        assert_eq!(vm.pc, 0);
        assert_eq!(vm.sp, 0xFF);
        assert_eq!(vm.mem, [0; MEMORY_SIZE]);
        assert_eq!(vm.is_halt, false);
    }

    #[test]
    fn fde_cycle() {}

    #[test]
    fn load_program() {}
}
