#include <QtTest/QtTest>

#include "cchip8/cpu.h"

// Macros are specific to this test
#define LOADDATA(dataPos, data) cpu->memory[MEM_START + (dataPos)] = (data)
#define EXECUTE QBENCHMARK_ONCE { step(cpu, interface); }
#define DISPLAY(x, y) interface->screen[x * DISPLAY_WIDTH + y]

class CPUTests : public QObject {
    Q_OBJECT

private slots:

    void init()
    {
        cpu = makeCPU();
        interface = makeInterface();
    }

    void cleanup()
    {
        free(cpu);
        free(interface);
    }

    // Snake_case for tests

    void test_CLS()
    {
        LOADDATA(0, 0x00);
        LOADDATA(1, 0xe0);
        interface->screen[0] = 1; // See if it really clears

        EXECUTE

        for (int i = 0 ; i < TOT_CHAR_COUNT ; ++i) { QCOMPARE(interface->screen[i], 0); }
    }

    void test_RET()
    {
        LOADDATA(0, 0x00);
        LOADDATA(1, 0xee);
        cpu->SP = 0x2;
        cpu->stack[0x2] = 0xf;

        EXECUTE

        QCOMPARE(cpu->PC, 0xf);
        QCOMPARE(cpu->SP, 0x1);
    }

    void test_JP_ADDR()
    {
        LOADDATA(0, 0x13);
        LOADDATA(1, 0x33);

        EXECUTE

        QCOMPARE(cpu->PC, 0x333);
    }

    void test_CALL_ADDR()
    {
        LOADDATA(0, 0x20);
        LOADDATA(1, 0x62);
        const uint16_t PC = cpu->PC;

        EXECUTE

        QCOMPARE(cpu->SP, 0);
        QCOMPARE(cpu->stack[cpu->SP], PC + 2);
        QCOMPARE(cpu->PC, 0x62);
    }

    void test_SE_VX_NN_NotEqual()
    {
        LOADDATA(0, 0x3a);
        LOADDATA(1, 0xbb);

        EXECUTE

        QCOMPARE(cpu->PC, 0x202);
    }

    void test_SE_VX_NN_Equal()
    {
        LOADDATA(0, 0x3a);
        LOADDATA(1, 0xbb);
        cpu->registers[0xa] = 0xbb;

        EXECUTE

        QCOMPARE(cpu->PC, 0x204);
    }

    void test_SNE_VX_NN_NotEqual()
    {
        LOADDATA(0, 0x4a);
        LOADDATA(1, 0xcc);

        EXECUTE

        QCOMPARE(cpu->PC, 0x204);
    }

    void test_SNE_VX_NN_Equal()
    {
        LOADDATA(0, 0x4a);
        LOADDATA(1, 0xcc);
        cpu->registers[0xa] = 0xcc;

        EXECUTE

        QCOMPARE(cpu->PC, 0x202);
    }

    void test_SE_VX_VY_Equal()
    {
        LOADDATA(0, 0x5a);
        LOADDATA(1, 0xb0);
        cpu->registers[0xa] = 0x5;
        cpu->registers[0xb] = 0x5;

        EXECUTE

        QCOMPARE(cpu->PC, 0x204);
    }

    void test_SE_VX_VY_NotEqual()
    {
        LOADDATA(0, 0x5a);
        LOADDATA(1, 0xb0);
        cpu->registers[0xa] = 0x5;
        cpu->registers[0xb] = 0x6;

        EXECUTE

        QCOMPARE(cpu->PC, 0x202);
    }

    void test_LD_VX_NN()
    {
        LOADDATA(0, 0x6a);
        LOADDATA(1, 0xbb);
        cpu->registers[0xa] = 0x10;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0xbb);
    }

    void test_ADD_VX_NN()
    {
        LOADDATA(0, 0x7a);
        LOADDATA(1, 0xbb);
        cpu->registers[0xa] = 0x10;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0x10 + 0xbb);
    }

    void test_LD_VX_VY()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb0);
        cpu->registers[0xb] = 0x8;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0x8);
    }

    void test_OR_VX_VY()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb1);
        cpu->registers[0xa] = 0x3;
        cpu->registers[0xb] = 0x4;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0x7);
    }

    void test_AND_VX_VY()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb2);
        cpu->registers[0xa] = 0x3;
        cpu->registers[0xb] = 0x4;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0);
    }

    void test_XOR_VX_VY()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb3);
        cpu->registers[0xa] = 0x3;
        cpu->registers[0xb] = 0x3;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0);
    }

    void test_ADD_VX_VY_NoCarry()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb4);
        cpu->registers[0xa] = 0x3;
        cpu->registers[0xb] = 0x4;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0x7);
        QCOMPARE(cpu->registers[0xf], 0);
    }

    void test_ADD_VX_VY_Carry()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb4);
        cpu->registers[0xa] = 0xff;
        cpu->registers[0xb] = 0xff;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0xfe);
        QCOMPARE(cpu->registers[0xf], 1);
    }

    void test_SUB_VX_VY_Carry()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb5);
        cpu->registers[0xa] = 0x4;
        cpu->registers[0xb] = 0x2;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 2);
        QCOMPARE(cpu->registers[0xf], 1);
    }

    void test_SUB_VX_VY_NoCarry()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb5);
        cpu->registers[0xa] = 0x2;
        cpu->registers[0xb] = 0x3;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 255);
        QCOMPARE(cpu->registers[0xf], 0);
    }

    void test_SHR_VX_VY_SIG1()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb6);
        cpu->registers[0xa] = 0x3;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0x3 >> 1);
        QCOMPARE(cpu->registers[0xf], 1);
    }

    void test_SUBN_VX_VY_NoCarry()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb7);
        cpu->registers[0xa] = 0x3;
        cpu->registers[0xb] = 0x2;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 255);
        QCOMPARE(cpu->registers[0xf], 0);
    }

    void test_SUBN_VX_VY_Carry()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xb7);
        cpu->registers[0xa] = 0x2;
        cpu->registers[0xb] = 0x3;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 1);
        QCOMPARE(cpu->registers[0xf], 1);
    }

    void test_SHL_VX_VY()
    {
        LOADDATA(0, 0x8a);
        LOADDATA(1, 0xbe);
        cpu->registers[0xa] = 0x3;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0x3 << 1);
        QCOMPARE(cpu->registers[0xf], 0);
    }

    void test_SNE_VX_VY_NotEqual()
    {
        LOADDATA(0, 0x9a);
        LOADDATA(1, 0xb0);
        cpu->registers[0xa] = 0x3;
        cpu->registers[0xb] = 0x4;

        EXECUTE

        QCOMPARE(cpu->PC, 0x204);
    }

    void test_SNE_VX_VY_Equal()
    {
        LOADDATA(0, 0x9a);
        LOADDATA(1, 0xb0);
        cpu->registers[0xa] = 0x3;
        cpu->registers[0xb] = 0x3;

        EXECUTE

        QCOMPARE(cpu->PC, 0x202);
    }

    void test_LD_I_ADDR()
    {
        LOADDATA(0, 0xa9);
        LOADDATA(1, 0x99);

        EXECUTE

        QCOMPARE(cpu->I, 0x999);
    }

    void test_JP_V0_ADDR()
    {
        LOADDATA(0, 0xb3);
        LOADDATA(1, 0x00);
        cpu->registers[0x0] = 0x2;

        EXECUTE

        QCOMPARE(cpu->PC, 0x2 + 0x300);
    }

    void test_DRW_VX_VY_N()
    {
        LOADDATA(0, 0xd1);
        LOADDATA(1, 0x25);
        cpu->registers[0x1] = 1;
        cpu->registers[0x2] = 1;

        EXECUTE

        QCOMPARE(DISPLAY(1, 1), 1);
        QCOMPARE(DISPLAY(2, 1), 1);
        QCOMPARE(DISPLAY(3, 1), 1);
        QCOMPARE(DISPLAY(4, 1), 1);
        QCOMPARE(DISPLAY(2, 2), 0);
        QCOMPARE(DISPLAY(4, 2), 0);
        QCOMPARE(DISPLAY(4, 3), 0);
        QCOMPARE(DISPLAY(5, 1), 1);
        QCOMPARE(cpu->registers[0xf], 0);

        LOADDATA(2, 0xd1);
        LOADDATA(3, 0x25);
        cpu->registers[0x1] = 1;
        cpu->registers[0x2] = 1;

        EXECUTE

        QCOMPARE(DISPLAY(1, 1), 0);
        QCOMPARE(DISPLAY(2, 1), 0);
        QCOMPARE(DISPLAY(3, 1), 0);
        QCOMPARE(DISPLAY(4, 1), 0);
        QCOMPARE(DISPLAY(2, 2), 0);
        QCOMPARE(DISPLAY(4, 2), 0);
        QCOMPARE(DISPLAY(4, 3), 0);
        QCOMPARE(DISPLAY(5, 1), 0);
        QCOMPARE(cpu->registers[0xf], 1);
    }

    void test_SKP_VX_Selected()
    {
        LOADDATA(0, 0xea);
        LOADDATA(1, 0x9e);
        cpu->registers[0xa] = 4;
        setKey(interface, 4);

        EXECUTE

        QCOMPARE(cpu->PC, 0x204);
    }

    void test_SKP_VX_NotSelected()
    {
        LOADDATA(0, 0xea);
        LOADDATA(1, 0x9e);
        cpu->registers[0xa] = 1;
        setKey(interface, 4);

        EXECUTE

        QCOMPARE(cpu->PC, 0x202);
    }

    void test_SKNP_VX_Selected()
    {
        LOADDATA(0, 0xeb);
        LOADDATA(1, 0xa1);
        cpu->registers[0xb] = 4;
        setKey(interface, 4);

        EXECUTE

        QCOMPARE(cpu->PC, 0x202);
    }

    void test_SKNP_VX_NotSelected()
    {
        LOADDATA(0, 0xeb);
        LOADDATA(1, 0xa1);
        cpu->registers[0xb] = 1;
        setKey(interface, 4);

        EXECUTE

        QCOMPARE(cpu->PC, 0x204);
    }

    void test_LD_VX_DT()
    {
        LOADDATA(0, 0xfa);
        LOADDATA(1, 0x07);
        cpu->DT = 0xf;

        EXECUTE

        QCOMPARE(cpu->registers[0xa], 0xf);
    }

    void test_LD_VX_N()
    {
        LOADDATA(0, 0xfb);
        LOADDATA(1, 0x0a);
        LOADDATA(2, 0xfa);
        LOADDATA(3, 0x07);
        setKey(interface, 5);

        EXECUTE

        QCOMPARE(cpu->registers[0xb], 5);
    }

    void test_LD_DT_VX()
    {
        LOADDATA(0, 0xfb);
        LOADDATA(1, 0x15);
        cpu->registers[0xb] = 0xf;

        EXECUTE

        QCOMPARE(cpu->DT, 0xf);
    }

    void test_LD_ST_VX()
    {
        LOADDATA(0, 0xfa);
        LOADDATA(1, 0x18);
        cpu->registers[0xa] = 0xf;

        EXECUTE

        QCOMPARE(cpu->ST, 0xf);
    }

    void test_ADD_I_VX()
    {
        LOADDATA(0, 0xfa);
        LOADDATA(1, 0x1e);
        cpu->I = 0xe;
        cpu->registers[0xa] = 0xf;

        EXECUTE

        QCOMPARE(cpu->I, 0xe + 0xf);
    }

    void test_LD_F_VX()
    {
        LOADDATA(0, 0xfa);
        LOADDATA(1, 0x29);
        cpu->registers[0xa] = 0xa;

        EXECUTE

        QCOMPARE(cpu->I, 0xa * 5);
    }

    void test_LD_B_VX()
    {
        LOADDATA(0, 0xfa);
        LOADDATA(1, 0x33);
        cpu->registers[0xa] = 0x7b;
        cpu->I = 0x300;

        EXECUTE

        QCOMPARE(cpu->memory[0x300], 1);
        QCOMPARE(cpu->memory[0x301], 2);
        QCOMPARE(cpu->memory[0x302], 3);
    }

    void test_LD_I_VX()
    {
        LOADDATA(0, 0xfb);
        LOADDATA(1, 0x55);
        cpu->I = 0x400;
        for (uint8_t i = 0; i <= 0xb; i++) {
          cpu->registers[i] = i;
        }

        EXECUTE

        for (uint8_t i = 0; i <= 0xb; i++) {
          QCOMPARE(cpu->memory[cpu->I + i], i);
        }
        QCOMPARE(cpu->memory[cpu->I + 0xc], 0);
    }

    void test_LD_VX_I()
    {
        LOADDATA(0, 0xfb);
        LOADDATA(1, 0x65);
        cpu->I = 0x400;
        for (uint8_t i = 0; i <= 0xa; i++) {
          cpu->memory[cpu->I + i] = i;
        }

        EXECUTE

        for (uint8_t i = 0; i <= 0xa; i++) {
          QCOMPARE(cpu->registers[i], i);
        }
        QCOMPARE(cpu->registers[cpu->I + 0xb], 0);
    }


private:
    CPU* cpu;
    Interface* interface;
};

QTEST_MAIN(CPUTests)

#include "cputests.moc"
