// asm.rs

// 
// We have 15 operations 0x0 -> 0xF.
// One is Reserved
// 
#[derive(Debug)]
pub enum Op {
    Nop,
    // Add,
    // Sub,
    // Indx,
    // Ld,
    // St,
    // Cls,
    // Spx,
    // Jmp,
    // Call,
    // Ret,
    // Res0, // Reserved
    // Res1, // Reserved
    // Res2, // Reserved
    // Halt,
}

#[derive(Debug)]
pub struct Instruction {
    op: Op,
    dest: u8,
    hn: u8,
    ln: u8,
}

impl Instruction {
    pub fn new(op: Op) -> Self {
        Instruction {
            op: op,
            dest: 0,
            hn: 0,
            ln: 0,
        }
    }
}


