
void commit_TC0(const uint8_t *values, uint8_t size);
void commit_TC1(const uint8_t *values, uint8_t size);
void commit_TC1_ICR(const uint8_t *values, uint8_t size);
void commit_TC2(const uint8_t *values, uint8_t size);
void commit_manchester(const uint8_t *values, uint8_t size);

#define commit commit_TC1_ICR

#define cnt 15



uint8_t data[15] = { 0xf0, 0x09, 0x69, 0x09, 0x69, 0x09, 0x69, 0x09, 0x69, 0x09, 0x69, 0x09, 0x69, 0x09, 0x61 };




int main() {
  CLKPR = _BV(CLKPCE);    // enable clock prescaler changes
  CLKPR = _BV(CLKPS2);    // set clock prescaler

do {
    data[0] = 0x0;
    commit(data, cnt);

    data[0] = 0x1;
    commit(data, cnt);

    data[0] = 0x2;
    commit(data, cnt);

    data[0] = 0x4;
    commit(data, cnt);
    
    data[0] = 0x8;
    commit(data, cnt);

    data[0] = 0x10;
    commit(data, cnt);

    data[0] = 0x20;
    commit(data, cnt);

    data[0] = 0x40;
    commit(data, cnt);

    data[0] = 0x80;
    commit(data, cnt);
    _delay_ms(1); 
  } while (true);
}
