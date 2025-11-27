// isa.rs

use crate::CPU;

// we want to implement the instructions.

// We have 15 operations 0x0 -> 0xF.
#[derive(Debug)]
pub enum Op {
    Nop,
    Add,
    Sub,
    Indx,
    Ld,
    St,
    Cls,
    Spx,
    Jmp,
    Call,
    Ret,
    Res0, // Reserved
    Res1, // Reserved
    Res2, // Reserved
    Res3, // Reserved
    Halt,
}

impl From<u8> for Op {
    fn from(opcode: u8) -> Self {
        return match opcode {
            0x0 => Op::Nop,
            0x1 => Op::Add,
            0x2 => Op::Sub,
            0x3 => Op::Indx,
            0x4 => Op::Ld,
            0x5 => Op::St,
            0x6 => Op::Cls,
            0x7 => Op::Spx,
            0x8 => Op::Jmp,
            0x9 => Op::Call,
            0xA => Op::Ret,
            0xB => Op::Res0,
            0xC => Op::Res1,
            0xD => Op::Res2,
            0xE => Op::Res3,
            0xF => Op::Halt,
            _ => panic!("Wow grossly out of bounds"),
        };
    }
}

#[derive(Debug)]
pub struct Instruction {
    op: Op,
    dest: u8,
    hn: u8,
    ln: u8,
}

impl From<u16> for Instruction {
    fn from(mem: u16) -> Self {
        let opcode: u8 = ((mem & 0xF000) >> 12) as u8;
        let dest: u8 = ((mem & 0xF00) >> 8) as u8;
        let hn: u8 = ((mem & 0xF0) >> 4) as u8;
        let ln: u8 = (mem & 0xF) as u8;
        Instruction {
            op: Op::from(opcode),
            dest,
            hn,
            ln,
        }
    }
}

impl Instruction {
    pub fn execute(&self, cpu: &mut CPU, mem: &mut [u16; 32768]) {
        let (dest, hn, ln) = (self.dest as usize, self.hn as usize, self.ln as usize);
        match self.op {
            Op::Nop => ()
            Op::Add => { cpu.reg[dest] = cpu.reg[hn] + cpu.reg[ln] }
            Op::Sub => { cpu.reg[dest] = cpu.reg[hn] - cpu.reg[ln] }
            Op::Indx => {
                cpu.i = (((self.dest as u16) & 0xF)  << 12) | (((self.hn as u16) & 0xF) << 8) | ((self.ln as u16) & 0xF)
            }
            // Op::Ld => {}
            Op::St => {
                // vm->mem[vm->I + dest] = (hn << 4) | ln;
                let address = cpu.i + self.dest as u16;
                mem[address as usize] = ((self.hn << 4) | self.ln) as u16;
            }
            // Op::Cls => {}
            // Op::Spx => {}
            // Op::Jmp => {}
            // Op::Call => {}
            // Op::Ret => {}
            // Op::Res0 => {}
            // Op::Res1 => {}
            // Op::Res2 => {}
            // Op::Res3 => {}
            Op::Halt => { cpu.is_halted = true }
            _ => panic!("Not implemented")
        }
    }
}
