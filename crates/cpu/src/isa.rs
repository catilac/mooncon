// isa.rs

// we want to implement the instructions.

// We have 15 operations 0x0 -> 0xF.
// One is Reserved
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
    Halt,
}

trait Exec {
    fn exec()
}

#[derive(Debug)]
pub struct Instruction {
    pub op: Op,
    dest: u8,
    hn: u8,
    ln: u8,
}
