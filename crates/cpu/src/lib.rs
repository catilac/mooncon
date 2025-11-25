// We have 15 operations 0x0 -> 0xF.
// One is Reserved
#[derive(Debug)]
enum Op {
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
    Halt,
}

#[derive(Debug)]
struct Instruction {
    op: Op,
    dest: u8,
    hn: u8,
    ln: u8,
}

fn add(left: u64, right: u64) -> u64 {
    left + right
}

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

    pub fn fde_cycle(cpu: &mut CPU, mem: [u16; 32768]) {
        let m = cpu.fetch(mem);
        let instruction = cpu.decode(m);
        cpu.execute(instruction);
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

    fn execute(&self, _instr: Instruction) {}
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn fde_cycle() {
        let mut cpu = CPU::init();
        CPU::fde_cycle(&mut cpu, [0u16; 32768]);
        assert!(true);
    }
}
