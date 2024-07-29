# 🧭 C-NORTH 🧭
<p align="center">
  <img src="logo.png" alt="Sublime's custom image" width="200"/>
</p>

## 📚 North (C version) is a very basic, stack based programming language written in c. Inspired by forth, porth, asm and SQL!
### This was my first somewhat bigger project in c. I have to learn a lot more, but nonetheless north was a good experience to make (sometimes) and i will continue working on the c version of it.

Version: **BETA0.2**

> 📌 I'm working on __*north+*__, therefore this project currently on hiatus!

- [x] Turing complete
- [ ] Optimalized code
- [ ] Error Handling
- [ ] Working strings 🔴 strings currently can't have spaces because of the primitive lexer.

## Features & Syntax
- Entry point, and basic operations

This code snippet pushes 10 int-s to the stack, adding them togheter and showing poping the top element.

 ```ruby
label main:
    PUSH 10, PUSH 20, ADD, TOP
    EXIT
end
```

 ```ruby
label main:
    PUSH 10, PUSH 20, SUB, TOP
    PUSH 10, PUSH 20, MUL, TOP
    PUSH 10, PUSH 20, DIV, TOP
    EXIT
end
```

- Labels 

Labels are like goto statements, you can just run them. ~LB just means linebreak.

 ```ruby
label label_name:
    PRINT 'your-label' LB
    EXIT
end

label main:
    RUN label_name
end
```

- Input & If

INPUT just gets an 'int' input from the user. IF runs the label if the top element is 0. This i enough for turing completenes!

 ```ruby
label NULL:
    PRINT 'THIS-IS-NULL' LB
    EXIT
end

label main:
    INPUT # if the input is 0, the program jumps to the 'NULL' label and exits
    IF NULL
    PRINT 'NOT-NULL' LB # if not 0, the program prints this and exits
    EXIT
end
```

