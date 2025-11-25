// lib.rs

mod isa;

use isa::{Instruction, Op};

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
        let opcode: u8 = ((m & 0xF000) >> 12) as u8;
        let dest: u8 = ((m & 0xF00) >> 8) as u8;
        let hn: u8 = ((m & 0xF0) >> 4) as u8;
        let ln: u8 = (m & 0xF) as u8;
        Instruction {
            op: Op::Nop,
            dest,
            hn,
            ln,
        }
    }

    fn execute(&mut self, i: Instruction) {
        match i.op {
            Op::Nop => (),
            Op::Add => {
                self.reg[i.dest as usize] = self.reg[i.hn as usize] + self.reg[i.ln as usize];
            }
            Op::Indx => {
                //vm->I = (dest << 12) | (hn << 8) | ln;
                // TODO: is this the actual way to express this?
                self.i = ((i.dest as u16) << 12) | ((i.hn as u16) << 8) | i.ln as u16;
            }
            Op::St => {
                // vm->reg[dest] = (hn << 4) | ln;
                self.reg[i.dest as usize] = (i.hn << 4) | i.ln;
            }

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
