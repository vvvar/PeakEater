import { describe, expect, test } from '@jest/globals';
import { EnvironmentBuilder } from '@hexlabs/env-vars-ts';
import { readFileSync } from 'fs'

const environment = EnvironmentBuilder.create('COMPILE_COMMANDS_PATH').environment();

interface CompilationUnit {
    directory: string;
    command: string;
    file: string;
    output: string;
}

const testOnMac = process.platform === "darwin" ? test : test.skip;

describe('Compilation Database', () => {
    test('Generated', () => {
        expect(readFileSync(environment.COMPILE_COMMANDS_PATH, { encoding: 'utf-8' })).not.toBeUndefined();
    });
    test('Valid JSON', () => {
        expect(() => JSON.parse(readFileSync(environment.COMPILE_COMMANDS_PATH, { encoding: 'utf-8' }))).not.toThrow();
    });
    testOnMac('All compilation units supports both x86_64 and arm64', () => {
        const compileCommands: CompilationUnit[] = JSON.parse(readFileSync(environment.COMPILE_COMMANDS_PATH, { encoding: 'utf-8' }));
        for (const compilationUnit of compileCommands) {
            expect(compilationUnit.command).toContain("-arch x86_64")
            expect(compilationUnit.command).toContain("-arch arm64")
        }
    });
    testOnMac('All compilation units supports macOS 10.9+', () => {
        const compileCommands: CompilationUnit[] = JSON.parse(readFileSync(environment.COMPILE_COMMANDS_PATH, { encoding: 'utf-8' }));
        for (const compilationUnit of compileCommands) {
            expect(compilationUnit.command).toContain("-mmacosx-version-min=10.9")
        }
    });
});

