#include <stdint.h>
#include <windows.h>

#include "lib/bignum.h"

static const uint64_t bignum_tabl[] = {
    0x7B09302DB5F059E9, 0x6F38D971A22020A3, 0xCB02BF3F1BCADEA8,
    0x3F7B9D178470CCF4, 0xFF92BCD5394145C2, 0x37D2BC4049592299,
    0xE7BB4D1551F5F596, 0x6B01B1D13E88AAD6, 0x1F8541B9A6F4DDD6,
    0x276F7DA43FAA9F82, 0x234FD9F3420BBCC8, 0xF7CDC3F253967BE2,
    0xC3A1823DB84FBCE7, 0xAF6C502FFD2E75E1, 0x97DDFA6ED711A6CE,
    0xF3437A7B61D67181, 0x1B3E8110EA0A55AA, 0x8FDD3E47006773B1,
    0x1B35356C590E04B0, 0x0FC4BC3709938095, 0x5F3A4F11AC408C8E,
    0xF7B143490CEDFACC, 0x17996F45C8EAF1C8, 0xAFC4EA4EF3516EF1,
    0xF7801F184138C6E3, 0x1F991F3D31B7CBDC, 0xAB5EC3F4E4DD1FC6,
    0xAF3FBE9555E33ED3, 0x4389C3AAD407AE90, 0xDF66A02EBF3820A7,
    0x5F118C25533484FA, 0xEB2BA9939E2ED6F0, 0x73D00C65BCB179EB,
    0xA3EDFAC8FE32A2D7, 0x239A27820E97D681, 0xA7C281F76ECAF9DA,
    0x7B3661B66D40CEEA, 0xFFC32EFEC1FEBAA6, 0x4FF1C08F16965BF5,
    0x3BD5F3D40AB169B6, 0x83CA86B3523905FD, 0x3772D2D7917D1DA5,
    0xBF45F820FCF482E7, 0x4FD33196B8162EEE, 0xB36EB9FBDFDBDDE1,
    0xC3F6465BD1B079C7, 0x9FB0EB9C2785A7B5, 0xCB47777BD55B9F94,
    0x3B9EFBA3658065C9, 0xA7DF091B6ED99ACF, 0xCF00C6823B083AD4,
    0x2350F495E5D3F88D, 0xBF51DDA7D3ADEDFA, 0x53432EFF8A8146AB,
    0x5F68764CDA89A89C, 0x4F53689A66D61BAE, 0xBB9CC4285F7EEFB4,
    0x5FEDFC81B0809089, 0xEF10F7FBCEFD0F8C, 0x47A471474C5F0783,
    0x4F68FAB9775E0593, 0x5FECC9325D59C6C8, 0x0B0B8AA0BFFBB3B7,
    0x6356A53F5EE9858C, 0x37079C3CE761ACE2, 0xFB691CEF1D2E59F1,
    0xE33A55720C0606A9, 0x03EB7DE02DB2EFB6, 0x1739C13B9239BFD7,
    0x931A7806C51096E3, 0xC3B5BA5A978608BC, 0xDF260338C1C868F0,
    0xBF4677533238E7D4, 0x0B8274B8CF99BDD6, 0xCFD7B7069C3103AF,
    0x0B8BB0471BC6DC96, 0x234C98662158CE89, 0x8385FD239458EACC,
    0xA33B0EDA26046FFC, 0x9BAB1A30DF2AC3E0, 0x4B69382D740A8FF6,
    0xFFC1C830F7F27BEE, 0x7F12B0150F2A8690, 0x53382B1C5EB06AC9,
    0x9F6ACD7CB1BE1E82, 0xB39BA32E79FBF0AD, 0x9705B98939F11AA5,
    0xCB91B9557B1BCFFE, 0x07DD0C58CDEB5FCC, 0xFB0B96F6A490F6DC,
    0x73C69F2EABA694D2, 0xCF54D489C38F37FD, 0xD7DC7D1D68DE14B3,
    0x8B90EC318E3A90A9, 0x07742269018D7CF5, 0x1F529523A66148B1,
    0x57C4F503373A49C4, 0xDF250149B8638EAF, 0x7F0E44D517FA1E90,
    0xC78F9A55BB547793, 0x971B686006DC0D90, 0xDF0CEE9BA4EA14B1,
    0x6705991A59F7C8A9, 0xCB5561F954083DD6, 0x6F2A7FF9D1003EB0,
    0x8F496C0D76894EC1, 0x838F4A7261114AAE, 0x5B6C9A570DB963FE,
    0xF71801B6D90A08C2, 0x8F5AA53EE2610CC4, 0x8F268AD07E1C77D8,
    0xD3D711EA94C2E9BA, 0x8752E30E7A7E1EA0, 0x6F233EB986DEFDB8,
    0x17A399F5BB663488, 0x534B67B1C4DCD2C8, 0x576603E8D79E6FB9,
    0xD36563401CC4DB82, 0xAB701DB689A832DC, 0xB7286C424BFA31BB,
    0x67246ED430FBC9A0, 0x4F36B1034E814587, 0xC758AA567742F785,
    0x67F939F11D44018D, 0x6FE107FAA7BA53BE, 0xDF7E450E9AD15CAE,
    0xEF90AB93E039C6F1, 0xE3FEFDB198BA1CBF};

typedef union {
  uint64_t rx;
  uint32_t ex;
  uint16_t x;
  struct {
    uint8_t lo, hi;
  } b;
} Register;

extern void _stdcall d2k2_crackme08_hash(DWORD *output, DWORD input_len,
                                         DWORD *input);

void init() {}

void process_serial(char *name, char *serial_out) {
  TCHAR usrname[0x80] = {0};
  uint8_t haval_hash[0x80] = {0};
  uint8_t serialhash[0x10] = {0};
  DWORD compname_len = 0x80;
  GetUserNameA(usrname, &compname_len);
  compname_len = strlen(usrname);

  d2k2_crackme08_hash(haval_hash, compname_len, usrname);

  Register EB, EA, ED;
  uint8_t bignum_tabloff1;
  uint8_t bignum_tabloff2;
  uint8_t rotr_var = 8;
  uint8_t rotl_var = 0x10;
  uint32_t *hash_ptr = haval_hash;
  EA.ex = _rotr(*hash_ptr, 4);
  EB.ex = *(uint32_t *)(hash_ptr + 1);
  EA.ex ^= EB.ex;
  ED.ex = EA.ex % 0x80;
  EA.ex = EA.ex / 0x80;
  bignum_tabloff1 = ED.ex;
  ED.ex = EA.ex % 0x80;
  EA.ex = EA.ex / 0x80;
  bignum_tabloff2 = ED.ex;

  for (int i = 0; i < compname_len; i++) {
    rotr_var += ED.b.lo;
    rotl_var -= EA.b.lo;
  }

  uint32_t *ser_ptr = serialhash;
  uint8_t name_len = strlen(name);
  for (int i = 0; i < 4; i++) {
    int8_t chr = name[i];
    uint32_t hash_frag =
        _rotl(*(uint32_t *)((uint8_t *)hash_ptr + i) / chr, rotl_var);
    uint32_t name_frag = *(uint32_t *)name;
    hash_frag =
        _rotr(hash_frag ^ (0xEDB88320 ^ name_frag), rotr_var) - name_len;
    *(uint32_t *)(ser_ptr + i) = hash_frag;
  }

  mbedtls_mpi P, Q, E, N, temp_N, D, serialhash_bn, serial;

  mbedtls_mpi_init(&P);
  mbedtls_mpi_init(&Q);
  mbedtls_mpi_init(&E);
  mbedtls_mpi_init(&N);
  mbedtls_mpi_init(&D);
  mbedtls_mpi_init(&serial);
  mbedtls_mpi_init(&serialhash_bn);
  mbedtls_mpi_init(&temp_N);
  mbedtls_mpi_read_binary(&P, &bignum_tabl[bignum_tabloff1], 8);
  mbedtls_mpi_read_binary(&Q, &bignum_tabl[bignum_tabloff2], 8);
  mbedtls_mpi_read_binary(&serialhash_bn, serialhash, 8);
  mbedtls_mpi_read_string(&E, 16, "10001");
  // to get D we need  D=E^(-1) mod ((P-1)*(Q-1))
  // get N first
  mbedtls_mpi_mul_mpi(&N, &P, &Q);
  // so decrement P and Q :)
  mbedtls_mpi_sub_int(&P, &P, 1);
  mbedtls_mpi_sub_int(&Q, &Q, 1);
  // get temp_N which is ((P-1)*(Q-1))
  mbedtls_mpi_mul_mpi(&temp_N, &P, &Q);
  // to get D we need modular inverse!
  mbedtls_mpi_inv_mod(&D, &E, &temp_N);
  // we got D! :D now we use regular RSA operation (serial = input ^ D mod N)
  mbedtls_mpi_exp_mod(&serial, &serialhash_bn, &D, &N, NULL);

  int len;
  mbedtls_mpi_write_string(&serial, 16, serial_out, 0x80, &len);

  mbedtls_mpi_free(&P);
  mbedtls_mpi_free(&Q);
  mbedtls_mpi_free(&E);
  mbedtls_mpi_free(&N);
  mbedtls_mpi_free(&D);
  mbedtls_mpi_free(&serial);
  mbedtls_mpi_free(&serialhash_bn);
  mbedtls_mpi_free(&temp_N);
}
