// isa.rs

// we want to implement the instructions.

// We have 15 operations 0x0 -> 0xF.
// One is Reserved
// #[derive(Debug)]
// pub enum Op {
//     Nop,
//     Add,
//     Sub,
//     Indx,
//     Ld,
//     St,
//     Cls,
//     Spx,
//     Jmp,
//     Call,
//     Ret,
//     Res0, // Reserved
//     Res1, // Reserved
//     Res2, // Reserved
//     Halt,
// }

#[derive(Debug)]
pub struct Instruction {
    pub op: Op,
    pub dest: u8,
    pub hn: u8,
    pub ln: u8,
}

// impl Instruction {
//     fn decode(m: u16) -> Self {
//         let opcode: u8 = ((m & 0xF000) >> 12) as u8;
//         let dest: u8 = ((m & 0xF00) >> 8) as u8;
//         let hn: u8 = ((m & 0xF0) >> 4) as u8;
//         let ln: u8 = (m & 0xF) as u8;
//         let op =
//         Self { op, dest, hn, ln }
//     }
// }

struct Nop {}
struct Add {}
// struct Sub {}
struct Indx {}
// struct Ld {}
struct St {}
// struct Cls {}
// struct Spx {}
// struct Jmp {}
// struct Call {}
// struct Ret {}
// struct Res0 {} // Reserved
// struct Res1 {} // Reserved
// struct Res2 {} // Reserved
// struct Halt {}

trait Exec {
    fn exec(dest: u8, hn: u8, ln: u8);
}

impl Exec for Add {
    fn exec(dest: u8, hn: u8, ln: u8) {}
}
