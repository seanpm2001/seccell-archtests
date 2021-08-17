/* Permissions */
#define RT_R 0b00000010     /* 0x2 */
#define RT_W 0b00000100     /* 0x4 */
#define RT_X 0b00001000     /* 0x8 */

/* Exceptions */
#define RISCV_EXCP_INST_ACCESS_FAULT             0x1
#define RISCV_EXCP_ILLEGAL_INST                  0x2
//...
#define RISCV_EXCP_LOAD_ADDR_MIS                 0x4
#define RISCV_EXCP_LOAD_ACCESS_FAULT             0x5
//...
#define RISCV_EXCP_INST_PAGE_FAULT               0xc 
#define RISCV_EXCP_LOAD_PAGE_FAULT               0xd 
#define RISCV_EXCP_STORE_PAGE_FAULT              0xf 
//...
#define RISCV_EXCP_SECCELL_ILL_ADDR              0x18
#define RISCV_EXCP_SECCELL_ILL_PERM              0x19
#define RISCV_EXCP_SECCELL_INV_SDID              0x1a
#define RISCV_EXCP_SECCELL_INV_CELL_STATE        0x1b
#define RISCV_EXCP_SECCELL_ILL_TGT               0x1c

static inline 
void set_urid(uint64_t urid) {
   asm("csrw urid, %[urid]"
      :: [urid] "r" (urid)
      :);
}

static inline 
uint64_t get_urid(void) {
   uint64_t urid;
   asm("csrr %[urid], urid"
      : [urid] "=r" (urid)
      ::);
   return urid;
}

static inline 
void set_usid(uint64_t usid) {
   asm("csrw usid, %[usid]"
      :: [usid] "r" (usid)
      :);
}

static inline 
uint64_t get_usid(void) {
   uint64_t usid;
   asm("csrr %[usid], usid"
      : [usid] "=r" (usid)
      ::);
   return usid;
}

/* Macros for assembly instructions */
/* Note: even though some of the instructions could be wrapped into static inline functions, macros were deliberately
   chosen to have unified calling conventions (similar to actual assembly instruction syntax). This is not very good
   software engineering practice and should be reworked in the future. */
#define nop(N)            \
   do {                   \
      asm(".rept " #N ";" \
          "nop;"          \
          ".endr"         \
          :::);           \
   } while(0)

#define entry(label)       \
   do {                    \
      asm (                \
         #label ":   \n\t" \
         "entry"           \
      );                   \
   } while (0)

#define jalrs(ret_reg, dest_reg, sd_reg)            \
   do {                                             \
      asm volatile (                                \
         "jalrs %[ret], %[dest], %[sd]"             \
         : [ret] "=r" (ret_reg)                     \
         : [dest] "r" (dest_reg), [sd] "r" (sd_reg) \
      );                                            \
   } while (0)

#define jals(sd_reg, dest_label)    \
   do {                             \
      asm volatile (                \
         "jals %[sd], " #dest_label \
         : [sd] "+r" (sd_reg)       \
         :                          \
      );                            \
   } while (0)

#define grant(addr_reg, sd_reg, perms_imm)                                    \
   do {                                                                       \
      asm volatile (                                                          \
         "grant %[addr], %[sd], %[perms]"                                     \
         :                                                                    \
         : [addr] "r" (addr_reg), [sd] "r" (sd_reg), [perms] "i" (perms_imm)  \
      );                                                                      \
   } while (0)

#define tfer(addr_reg, sd_reg, perms_imm)                                     \
   do {                                                                       \
      asm volatile (                                                          \
         "tfer %[addr], %[sd], %[perms]"                                      \
         :                                                                    \
         : [addr] "r" (addr_reg), [sd] "r" (sd_reg), [perms] "i" (perms_imm)  \
      );                                                                      \
   } while (0)

#define prot(addr_reg, perms_imm)                                    \
   do {                                                              \
      /* Attention: variable might shadow name from outer scope */   \
      uint64_t tmp_perms = (perms_imm);                              \
      asm volatile (                                                 \
         "prot %[addr], %[perms]"                                    \
         :                                                           \
         : [addr] "r" (addr_reg), [perms] "r" (tmp_perms)            \
      );                                                             \
   } while (0)

#define inval(addr_reg)          \
   do {                          \
      asm volatile (             \
         "inval %[addr]"         \
         :                       \
         : [addr] "r" (addr_reg) \
      );                         \
   } while (0)

#define reval(addr_reg, perms_imm)                                   \
   do {                                                              \
      /* Attention: variable might shadow name from outer scope */   \
      uint64_t tmp_perms = (perms_imm);                              \
      asm volatile (                                                 \
         "reval %[addr], %[perms]"                                   \
         :                                                           \
         : [addr] "r" (addr_reg), [perms] "r" (tmp_perms)            \
      );                                                             \
   } while (0)

#define count(cnt_reg, addr_reg, perms_imm)                          \
   do {                                                              \
      /* Attention: variable might shadow name from outer scope */   \
      uint64_t tmp_perms = (perms_imm);                              \
      asm (                                                          \
         "count %[cnt], %[addr], %[perms]"                           \
         : [cnt] "=r" (cnt_reg)                                      \
         : [addr] "r" (addr_reg), [perms] "r" (tmp_perms)            \
      );                                                             \
   } while (0)

#define csrr_usid(usid_reg)         \
   do {                             \
      asm (                         \
         "csrr %[usid], usid"       \
         : [usid] "=r" (usid_reg)   \
         :                          \
      );                            \
   } while (0)
