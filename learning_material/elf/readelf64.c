#include<elf.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
int main(int argc,char **argv)
{
	long long ret;
	if(argc!=2)
	{
		printf("the number of parameter must be one\n");
		return 0;
	}

	FILE* fp = fopen(argv[0],"rb");
	assert(fp);
	
	/* print a Header*/
	printf("ELF header:\n");	
	
	//get elf header
	uint8_t buf[sizeof(Elf64_Ehdr)];
	ret = fread(buf,sizeof(Elf64_Ehdr),1,fp);
	Elf64_Ehdr *elf = (void*) buf;
	
	//print the elf header information
	if(elf->e_ident[0]!=0x7f||elf->e_ident[1]!='E'||elf->e_ident[2]!='L'||elf->e_ident[3]!='F')
	{
		printf("%s is not elf!\n",argv[0]);
		fclose(fp);
		return 0;
	}
	
	switch(elf->e_ident[4])
	{
		case 0:
			printf("Class: unrecognized type\n");
			break;
		case 1:
			printf("Class: 32-bit elf,only support 64-bit elf\n");
			fclose(fp);
			return 0;
		case 2:
			printf("Class: 64-bit elf\n");
	}
	
	switch(elf->e_ident[5])
	{
		case 0:
			printf("Coding format: illegal\n");
			break;
		case 1:
			printf("Coding format: LSB\n");
			break;
		case 2:
			printf("Coding format: MSB\n");
	}	
	
	if(elf->e_ident[6]==1)
		printf("Veriosn: Current Version\n");
	else
		printf("Version: illegal\n");

	switch(elf->e_type)
	{
		case 0:
			printf("Type: unknown\n");
			break;
		case 1:
			printf("Type: relocatable file\n");
			break;
		case 2:
			printf("Type: executable file\n");
			break;
		case 3:
			printf("Type: dynamic shared file\n");
			break;
		case 4:
			printf("Type: Core\n");
			break;
	}

	
	switch(elf->e_machine)
	{
		case 2:
			printf("Machine: 80386\n");
			break;
		case 7:
			printf("Machine: Intel80860\n");
			break;
		case 62:
			printf("Machine: AMD 64bits\n");
			break;
		default:
			printf("Machine: %d",elf->e_machine);
	}
	
	printf("Veriosn: %d\n",elf->e_version);
	printf("Entry: %016lx\n",elf->e_entry);
	printf("Program Header Offset: %ld\n",elf->e_phoff);
	printf("Section Header Offset: %ld\n",elf->e_shoff);
	printf("ELF Header Size: %d\n",elf->e_ehsize);
	printf("Program Header Entry Size:%d\n",elf->e_phentsize);
	printf("Program Header Num: %d\n",elf->e_phnum);
	printf("Section Header Entry Size: %d\n",elf->e_shentsize);
	printf("Section Header Num:%d\n",elf->e_shnum);
	printf("String Name Table Index in Section Header: %d\n",elf->e_shstrndx);

	//Load symbol table and string table
	uint32_t sh_size =elf->e_shentsize * elf->e_shnum;
	Elf64_Shdr *sh = malloc(sh_size);
	fseek(fp,elf->e_shoff,SEEK_SET);
	ret = fread(sh,sh_size,1,fp);
	assert(ret == 1);
	
	//Load section header string table
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp,sh[elf->e_shstrndx].sh_offset,SEEK_SET);
	ret = fread(shstrtab,sh[elf->e_shstrndx].sh_size,1,fp);

	char *strtab = NULL;
	Elf64_Sym *symtab = NULL;
	int nr_symtab_entry;

	int len;
	for(int i=0;i<elf->e_shnum;i++)
	{
		if(sh[i].sh_type == SHT_SYMTAB && strcmp(shstrtab + sh[i].sh_name,".symtab")==0)
		{
			symtab = malloc(sh[i].sh_size);
			fseek(fp,sh[i].sh_offset,SEEK_SET);
			ret = fread(symtab,sh[i].sh_size,1,fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size/sizeof(symtab[0]);
		}
		else if(sh[i].sh_type == SHT_STRTAB && strcmp(shstrtab + sh[i].sh_name,".strtab")==0)
		{
			len = sh[i].sh_size;
			strtab = malloc(sh[i].sh_size);
			fseek(fp,sh[i].sh_offset,SEEK_SET);
			ret = fread(strtab,sh[i].sh_size,1,fp);
			assert(ret==1);
		}
	}
	
	printf("Symbol table:\n");
	printf(" %-40s %-10s %-10s %-10s %-10s %-10s \n","Name","Type","Visbility","Relation","Value","Size");
	for(int i=0;i<nr_symtab_entry;i++)
        {
		if(symtab[i].st_name!=0)
			printf(" %-40s ",&strtab[symtab[i].st_name]);
		else 
			printf(" %-40s ","No Name ");
                printf("%-10x ",symtab[i].st_info);
                printf("%-10x ",symtab[i].st_other);
                printf("%-10d ",symtab[i].st_shndx);
                printf("%-10lx ",symtab[i].st_value);
                printf("%-10ld ",symtab[i].st_size);
                printf("\n");
        }

	printf("\n");
	free(sh);
	free(shstrtab);	
	fclose(fp);
	return 0;
}
