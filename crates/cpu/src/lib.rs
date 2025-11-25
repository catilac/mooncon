// lib.rs

mod isa;

use isa::{Instruction, Op};

pub struct CPU {
    i: u16,
    reg: [i8; 16],
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

    pub fn fde_cycle(&mut self, mem: [u16; 32768]) {
        let m = self.fetch(mem);
        let instruction = self.decode(m);
        self.execute(instruction);
    }

    fn fetch(&mut self, mem: [u16; 32768]) -> u16 {
        let m: u16 = mem[self.pc as usize];
        self.pc += 1;
        return m;
    }

    fn decode(&self, m: u16) -> Instruction {
        Instruction {
            op: Op::Nop,
            dest: 0,
            hn: 0,
            ln: 0,
        }
    }

    fn execute(&self, instr: Instruction) {
        match instr.op {
            Op::Nop => (),

            default => {
                todo!("Not Implemented")
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn fde_cycle() {
        let mut cpu = CPU::init();
        cpu.fde_cycle([0u16; 32768]);
        assert!(true);
    }
}
