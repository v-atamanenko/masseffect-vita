typedef void PosixFileSystem;
typedef void basic_string;
typedef void FileStream;
typedef signed char byte;
typedef void FileOutputStream;
typedef int32_t undefined4;
typedef void VFS;
typedef void* IOutputStream;

char path_tmp[1024]; // Helper static var for VFS::OpenOutputStream

void* (*operator_new_brackets)(uint p1, char* p2, int p3, uint p4, char* p5, int p6);
void* (*operator_new)(uint p1);
void (*EA__IO__FileStream__FileStream)(FileStream * p1, char* p2);
void (*EA__IO__FileStream__Open)(FileStream * p1, int p2, int p3, int p4, int p5);
void (*operator_delete_brackets)(void *param_1);
void (*FileOutputStream__FileOutputStream)(FileOutputStream* p1, FileStream * p2);

int **PTR_ARRAY_00a58e78;

PosixFileSystem * im__PosixFileSystem__OpenOutputStream(PosixFileSystem *this,basic_string *param_1,char *param_2) {
    int *this_00;
    FileOutputStream *this_01;
    undefined4 *puVar3;
    char *pcVar4;
    char *local_28 [2];
    int local_20;

    pcVar4 = *(char **)param_2;
    this_00 = (int *)operator_new(0xa8);
    EA__IO__FileStream__FileStream((FileStream *)this_00,pcVar4);
    EA__IO__FileStream__Open(this_00, 3, 2, 1, 0);
    this_01 = (FileOutputStream *)operator_new(0xc);
    FileOutputStream__FileOutputStream(this_01,(FileStream *)this_00);
    *(FileOutputStream **)this = this_01;
    *(undefined4 *)(this + 4) = 0;
    puVar3 = (undefined4 *)operator_new_brackets(0x14,(char *)0x0,0,0,(char *)0x0,0);

    if (puVar3 != (undefined4 *)0x0) {
        *(ushort *)(puVar3 + 3) = *(ushort *)(puVar3 + 3) & 0x8000 | 0x10;
        *(byte *)((int)puVar3 + 0xd) = *(byte *)((int)puVar3 + 0xd) & 0x7f;
        puVar3[2] = 1;
        *puVar3 = PTR_ARRAY_00a58e78;
        *(ushort *)(puVar3 + 3) = *(ushort *)(puVar3 + 3) & 0x8000 | 0x14;
        puVar3[4] = this_01;
        puVar3[1] = 1;
        *(byte *)((int)puVar3 + 0xd) = *(byte *)((int)puVar3 + 0xd) | 0x80;
        *(undefined4 **)(this + 4) = puVar3;
    }

    if ((1 < local_20 - (int)local_28[0]) && (local_28[0] != (char *)0x0)) {
        operator_delete_brackets(local_28[0]);
    }

    return this;
}

IOutputStream ** im__VFS__OpenOutputStream(IOutputStream **outstream,VFS *vfs,basic_string *path,bool param_4) {
    if (**(char **)path == '@') {
        log_error("Unsupported codepath for @");
        return outstream;
    }

    snprintf(path_tmp, 1024, DATA_PATH"assets%s", *(char**)path);
    char ** tmp = (char**)&path_tmp;
    char *** tmp2 = &tmp;
    fclose(fopen(path_tmp, "w"));

    im__PosixFileSystem__OpenOutputStream(outstream, *(int **)(vfs + 8), tmp2);

    return outstream;
}

void patch__output_streams__init() {
    operator_new_brackets = (uintptr_t)so_mod.text_base + 0x00357900;
    operator_new = (uintptr_t)so_mod.text_base + 0x00357a70;
    EA__IO__FileStream__FileStream = (uintptr_t)so_mod.text_base + 0x0091e4a0;
    EA__IO__FileStream__Open = (uintptr_t)so_mod.text_base + 0x00915704;
    operator_delete_brackets = (uintptr_t)so_mod.text_base + 0x003576dc;
    FileOutputStream__FileOutputStream = (uintptr_t)so_mod.text_base + 0x0045f498;

    PTR_ARRAY_00a58e78 = (uintptr_t)so_mod.text_base + 0x00a58e78;

    hook_addr((uintptr_t)so_mod.text_base + 0x003ef804, (uintptr_t)&im__VFS__OpenOutputStream );
    hook_addr((uintptr_t)so_mod.text_base + 0x0040daac, (uintptr_t)&im__PosixFileSystem__OpenOutputStream );
}
