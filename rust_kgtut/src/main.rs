use std::env;

fn gen_serial(entered_name: &str) -> String {
    //I like this part of Rust, to me its kinda neat, much better than C
    let mut serial = vec![0u8; 10];
    let name: &[u8] = entered_name.as_bytes();
    //didn't know how to define multiple loop variables in Rust, first code written in it!
    let mut ctr = 0;
    //Like this part of Rust too, easy iteration!
    //But unless I know how to define multiple loop vars, I find C and x86 ASM easier!
    for i in (1..6).rev() {
        //mov cl,byte ptr ds:[edx+ebx], get letter in name
        let ascii_val: u8 = name[ctr];
        /*
        Xor letter value with 0x29 and add the value of the counter

        xor cl,0x29
        add cl,al*/
        let v1 = (ascii_val ^ 0x29) + (i as u8);
        /*
        The following Rust code is a port of the following x86 assembly language
        in the crackme:

        cmp cl,0x41
        jl d2k2_crackme01.4012A6
        cmp cl,0x5A
        jg d2k2_crackme01.4012A6
        mov byte ptr ds:[edx+0x40313C],cl
        mov byte ptr ds:[edx+0x40313D],0x0
        inc dl
        dec al
        cmp al,0x0
        je d2k2_crackme01.4012AC
        jmp d2k2_crackme01.40127D
        mov cl,0x52
        add cl,al
        jmp d2k2_crackme01.40128F

        Basically if the value from v1 fits some criteria the result is written to the
        calc buffer which is used later.
        */
        serial[ctr] = if v1 < b'A' || v1 > b'Z' {
            0x52 + (i as u8)
        } else {
            v1
        };
        /*

        mov cl,byte ptr ds:[edx+ebx]
        xor cl,0x27
        add cl,al
        add cl,0x1*/
        let mut v2 = (ascii_val ^ 0x27) + (i as u8);
        v2 += 1;
        /*
        This code fragment reflects the last 5 portions of the serial buffer,
        rewritten in Rust.

        cmp cl,0x41
        jl d2k2_crackme01.4012DF
        cmp cl,0x5A
        jg d2k2_crackme01.4012DF
        mov byte ptr ds:[edx+0x403141],cl
        mov byte ptr ds:[edx+0x403142],0x0
        inc dl
        dec al
        cmp al,0x0
        je d2k2_crackme01.4012E5
        jmp d2k2_crackme01.4012B3
        mov cl,0x4D
        add cl,al
        jmp d2k2_crackme01.4012C8*/
        serial[ctr + 5] = if v2 < b'A' || v2 > b'Z' {
            0x4D + (i as u8)
        } else {
            v2
        };
        ctr += 1;
    }

    for i in 0..10 {
        let mut gen: u8 = serial[i] + 5;
        /*
            The following logic from the crackme is wrote into Rust,
            making sure whenever a certain bit of logic is done in the check
            the right char is appended to the serial buffer:

            lea eax,dword ptr ds:[typedserial]
            mov bl,byte ptr ds:[ecx+eax]
            mov dl,byte ptr ds:[ecx+calcedbuffromstep1]
            cmp bl,0x0
            je d2k2_crackme01.4013A6 //end of serial check
            add dl,0x5
            cmp dl,0x5A
            jg d2k2_crackme01.401341
            xor dl,0xC
            cmp dl,0x41
            jl d2k2_crackme01.401346
            cmp dl,0x5A
            jg d2k2_crackme01.40134C
            inc ecx
            cmp dl,bl
            je d2k2_crackme01.401313
            jmp d2k2_crackme01.401352
            sub dl,0xD
            jmp d2k2_crackme01.40132D
            mov dl,0x4B
            add dl,cl
            jmp d2k2_crackme01.40133A
            mov dl,0x4B
            sub dl,cl
            jmp d2k2_crackme01.40133A

            In this code fragment I translated the DL register to "gen" in Rust.
            The rest is simple math.
        */
        if gen > b'Z' {
            gen -= 0xD;
        }
        gen ^= 0xC;
        if gen < b'A' {
            gen = 0x4B + (i as u8);
        }
        if gen > b'Z' {
            gen = 0x4B - (i as u8);
        }
        serial[i] = gen;
    }
    String::from_utf8(serial).expect("Found invalid UTF-8")
}

fn main() {
    //Check valid arguments
    let args: Vec<String> = env::args().collect();
    match args.len() {
        1 => {
            println!("Please input your name.");
        }
        2 => {
            let username = &args[1];
            //These name constraints are copied direct from crackme.
            if username.len() < 5 || username.len() > 32 {
                println!("Invalid name length!");
            } else {
                println!("Serial: {}", gen_serial(username));
            }
        }
        _ => {
            println!("Please input your name.");
        }
    }
}
