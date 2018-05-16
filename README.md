# Simple-SIC-Assembler

It is simple sic assembler to get the assembly code and generate the object code.  
When it is executed, you enter a file called temp and it comes out as follows.

### Input(temp file)

>test     start   1000  
first    lda     zero  
         sta     len  
         lda     zero  
         sta     result  
         jsub    strlen  
         jsub    strcpy  
         jsub    strcmp  
         lda     result  
         j       end  
strlen   ldx     len  
         ldch    str2  
         comp    null  
         jeq     back  
         lda     len  
         add     one  
         sta     len  
         j       strlen  
back     rsub  
strcpy   ldx     zero  
cloop    ldch    str2  
         stch    str1  
         tix     len  
         jlt     cloop  
         rsub  
strcmp   ldx     zero  
cploop   ldch    str2  
         comp    str3  
         jlt     small  
         jgt     big  
         jeq     equal  
small    lda     minus  
         sta     result  
         j       cend  
big      lda     plus  
         sta     result  
         j       cend  
equal    tix     len  
         jlt     cploop  
cend     rsub  
zero     word    0  
one      word    1  
minus    word    -1  
plus     word    1  
len      resw    1  
result   resw    1  
str1     resb    10  
str2     byte    c'test string'  
null     word    0  
str3     byte    c'test string'  
end      end     first  

### result(text.txt)

>Htest 00100000213d  
T0010001b0010750c10810010750c108448101b4810364810480010843c10aa041081  
T00101e1b50109128109c301033001081181810780c10813c101b4c0000041075501091  
T00103c1b5410872c10813810394c000004107550109128109f38105a34106330106c  
T00105a1b00107b0c10843c107200107e0c10843c10722c108138104b4c0000  
E001000    
  
(Note that it may not be perfect)
