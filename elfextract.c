#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define EI_NIDENT (16)
#define FILE_TYPE_LEN 4

struct elf32_header { 
	unsigned char e_ident[EI_NIDENT];  /* Magic number and other info */ 
	uint16_t	e_type;              /* Object file type */ 
	uint16_t	e_machine;           /* Architecture */ 
	uint32_t	e_version;           /* Object file version */ 
	uint32_t	e_entry;             /* Entry point virtual address */ 
	uint32_t	e_phoff;             /* Program header table file offset */ 
	uint32_t	e_shoff;             /* Section header table file offset */ 
	uint32_t	e_flags;             /* Processor-specific flags */ 
	uint16_t	e_ehsize;            /* ELF header size in bytes */ 
	uint16_t	e_phentsize;         /* Program header table entry size */ 
	uint16_t	e_phnum;             /* Program header table entry count */ 
	uint16_t	e_shentsize;         /* Section header table entry size */ 
	uint16_t	e_shnum;             /* Section header table entry count */ 
	uint16_t	e_shstrndx;          /* Section header string table index */ 
};

struct program_header {
	uint32_t	p_type;                 /* Segment type */
	uint32_t	p_offset;               /* Segment file offset */
	uint32_t	p_vaddr;                /* Segment virtual address */
	uint32_t	p_paddr;                /* Segment physical address */
	uint32_t	p_filesz;               /* Segment size in file */
	uint32_t	p_memsz;                /* Segment size in memory */
	uint32_t	p_flags;                /* Segment flags */
	uint32_t	p_align;                /* Segment alignment */
};

int main(int argc, char* argv[])
{
	FILE *in_filp;
	FILE *out_filp;
	struct elf32_header elf_header;
	struct program_header pg_header;
	char *in_file_path;
	char *out_file_path;
	char file_type[FILE_TYPE_LEN];
	int num_ph;
	char *buf;

	in_file_path = argv[1];
	out_file_path = argv[2];
	in_filp = fopen(in_file_path,"r");
	if (!in_filp) {
		printf("unable to load elf file:%s\n", in_file_path);
		return -1;
	}
	out_filp = fopen(out_file_path,"w+");
	if (!out_filp) {
		printf("unable to get output file:%s\n", out_file_path);
		return -1;
	}
	/* read file type */
	fread(&elf_header, sizeof(struct elf32_header), 1, in_filp);
	snprintf(file_type, FILE_TYPE_LEN, "%c%c%c", elf_header.e_ident[1], 
			elf_header.e_ident[2], elf_header.e_ident[3]);
	if (strcmp(file_type, "ELF")) {
		printf("%s is not elf format\n", in_file_path);
		return -1;
	}
	/* read program headers */
	fseek(in_filp, elf_header.e_phoff, SEEK_SET);
#if 0
	num_ph = elf_header.e_phnum;
	while(num_ph--) {
		fread(&pg_header, elf_header.e_phentsize, 1, in_filp);
		printf("type:%x\n", pg_header.p_type);
		printf("offset:%x\n", pg_header.p_offset);
		printf("vaddr:%x\n", pg_header.p_vaddr);
		printf("filesz:%x\n", pg_header.p_filesz);
		printf("memsz:%x\n", pg_header.p_memsz);
	}
#endif
	fread(&pg_header, elf_header.e_phentsize, 1, in_filp);
	buf = malloc(pg_header.p_filesz);
	fseek(in_filp, pg_header.p_offset, SEEK_SET);
	fread(buf, pg_header.p_filesz, 1, in_filp);
	printf("%x%x%x%x%x",buf[0],buf[1],buf[2],buf[3],buf[4]);
	fwrite(buf, pg_header.p_filesz, 1, out_filp);
	fclose(out_filp);
	fclose(in_filp);
	return 0;
}
