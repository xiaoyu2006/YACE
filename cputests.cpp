#include <QtTest/QtTest>

#include "cchip8/cpu.h"

// Macros are specific to this test
#define LOADDATA(dataPos, data) cpu->memory[MEM_START + (dataPos)] = (data)
#define EXECUTE QBENCHMARK_ONCE { step(cpu, interface); }

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

    // TODO: Finishi unittests


private:
    CPU* cpu;
    Interface* interface;
};

QTEST_MAIN(CPUTests)

#include "cputests.moc"
