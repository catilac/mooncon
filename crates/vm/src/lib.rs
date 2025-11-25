use cpu::CPU;

const MEMORY_SIZE: usize = 2 << 14;
const _STACK: usize = 2 << 12;
const _VRAM: usize = 2 << 13;
const _DISPLAY_SIZE: usize = 2048;

pub struct VM {
    cpu: CPU,
    mem: [u16; MEMORY_SIZE],
    is_halt: bool,
}

impl VM {
    pub fn init() -> Self {
        VM {
            cpu: CPU::init(),
            mem: [0; MEMORY_SIZE],
            is_halt: false,
        }
    }

    // TODO
    pub fn _load_program(&mut self) {}
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn initialization() {
        let vm = VM::init();
        assert_eq!(vm.mem, [0; MEMORY_SIZE]);
        assert_eq!(vm.is_halt, false);
    }

    #[test]
    fn load_program() {}
}
