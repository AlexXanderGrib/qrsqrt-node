(module 
  (func $Q_rsqrt (export "Q_rsqrt") (param $number f32) (result f32)
    (local $x f32)

    (local.set $x 
      (f32.reinterpret_i32 
        (i32.sub 
          (i32.const 0x5f375a86) 
          (i32.shr_s (i32.reinterpret_f32 (local.get $number)) (i32.const 1))
        )
      )
    )

    (f32.mul
      (local.get $x)
      (f32.sub 
        (f32.const 1.5) 
        (f32.mul (f32.mul (local.get $number) (f32.const 0.5)) (f32.mul (local.get $x) (local.get $x)))
      )
    )
  )

  (func $rsqrt (export "rsqrt") (param $number f32) (result f32) 
    (f32.div (f32.const 1)  (f32.sqrt (local.get $number)))
  )

  (memory (export "memory") 4)

  (func $Q_rsqrt_vec (export "Q_rsqrt_vec") (param $start i32) (param $length i32) 
    (local $pos i32)
    (local $end i32)

    (local.set $pos (local.get $start))
    (local.set $end (i32.add (local.get $start) (local.get $length)))

    (loop $loop
      (f32.store (local.get $pos) (call $Q_rsqrt (f32.load (local.get $pos))))

      (local.set $pos (i32.add (local.get $pos) (i32.const 4)))

      (br_if $loop (i32.lt_u (local.get $end) (local.get $pos)))
    )
  ) 
)
