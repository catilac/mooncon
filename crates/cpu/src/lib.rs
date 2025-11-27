// lib.rs

mod isa;

use isa::Instruction;

pub struct CPU {
    i: u16,
    reg: [u8; 16],
    pc: u16,
    sp: u8,
}

impl CPU {
    pub fn init() -> Self {
        CPU {
            i: 0,
            reg: [0; 16],
            pc: 0,
            sp: 0xFF,
        }
    }

    pub fn fde_cycle(&mut self, mem: &mut [u16; 32768]) {
        let m = self.fetch(mem);
        let instruction = self.decode(m);
        self.execute(instruction, mem);
    }

    fn fetch(&mut self, mem: &[u16; 32768]) -> u16 {
        let m: u16 = mem[self.pc as usize];
        self.pc += 1;
        return m;
    }

    fn decode(&self, m: u16) -> Instruction {
        return Instruction::from(m);
    }

    fn execute(&mut self, i: Instruction, mem: &mut [u16; 32768]) {
        i.execute(self, mem);
    }
}

#[cfg(test)]
mod tests {
    use super::CPU;

    #[test]
    fn fde_cycle() {
        let mut cpu = CPU::init();
        cpu.fde_cycle(&mut [0u16; 32768]);
        assert!(true);
    }
}
