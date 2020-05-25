#include "STD_TYPES.h"
#include <stdio.h>

/*=================================================================*/
                           /* Macros */
/*=================================================================*/
/* Elf_Header_t - Identity options */
#define MAG_0				(0)
#define MAG_1				(1)
#define MAG_2				(2)
#define MAG_3				(3)
#define CLASS				(4)
#define DATA				(5)
#define VERSION				(6)
#define PAD					(7)
#define IDENT				(16)

/* Elf_Header_t - Identity (CLASS)options */
#define CLASS_NONE			(0)
#define CLASS_32			(1)
#define CLASS_64			(2)

/* Elf_Header_t - type options */
#define TYPE_NONE  			(0)				/* No file type       */
#define TYPE_REL  			(1)             /* Relocatable file   */
#define TYPE_EXEC			(2)             /* Executable file    */
#define TYPE_DYN            (3)             /* Shared object file */
#define TYPE_CORE           (4)             /* Core file          */
#define TYPE_LOPROC         (0xff00)        /* Processor-specific */
#define TYPE_HIPROC         (0xffff)        /* Processor-specific */

/* Elf_Header_t - machine options */
#define MACHINE_NONE  			(0)				/* No machine             */
#define MACHINE_M32  			(1)             /* AT&T WE 32100          */
#define MACHINE_SPARC			(2)             /* SPARC                  */
#define MACHINE_386             (3)             /* Intel Architecture     */
#define MACHINE_68K             (4)             /* Motorola 68000         */
#define MACHINE_88K             (5) 	        /* Motorola 88000         */
#define MACHINE_860             (7)    		    /* Intel 80860            */
#define MACHINE_MIPS			(8)				/* MIPS RS3000 Big-Endian */
#define MACHINE_MIPS_RS4_BE		(10)			/* MIPS RS4000 Lttle-Endian*/
#define MACHINE_ARM				(40)			/* ARM					  */
/* Elf_Header_t - version options */
#define EV_NONE  				(0)				/* Invalid Version   */
#define EV_CURRENT  			(1)             /* Current version   */


/* Elf_Program_Header_t - type */
#define	TYPE_NULL				(0)
#define	TYPE_LOAD               (1)
#define	TYPE_DYNAMIC            (2)
#define	TYPE_INTERP             (3)
#define	TYPE_NOTE               (4)
#define	TYPE_SHLIB              (5)
#define	TYPE_PHDR               (6)

/* Elf_Program_Header_t - Flag */
#define FLAG_NONE				(0x0)
#define	FLAG_E					(0X1)
#define	FLAG_W               	(0X2)
#define	FLAG_WE            		(0X3)
#define	FLAG_R             		(0X4)
#define	FLAG_RE          	    (0X5)
#define	FLAG_RW              	(0X6)
#define	FLAG_RWE                (0X7)
/*=================================================================*/
						/* Definitions */
/*=================================================================*/
typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf32_Word;

typedef struct
{
    uint8_t  identity[16];
    Elf32_Half type;
    Elf32_Half machine;
    Elf32_Word version;
    Elf32_Addr entry;
    Elf32_Off  ph_offset;
    Elf32_Off  sh_offset;
    Elf32_Word flags;
    Elf32_Half header_size;
    Elf32_Half ph_size;
    Elf32_Half ph_num;
    Elf32_Half sh_size;
    Elf32_Half sh_num;
    Elf32_Half strtab_index;
}Elf_Header_t;


typedef struct
{
    Elf32_Word type;
    Elf32_Off  offset;
    Elf32_Addr virtual_address;
    Elf32_Addr physical_address;
    Elf32_Word file_size;
    Elf32_Word mem_size;
    Elf32_Word flags;
    Elf32_Word align;
}Elf_Program_Header_t;


typedef struct {
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off  sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} Elf_Section_Header_t;
/*=================================================================*/
						    /* Main */
/*=================================================================*/

uint8_t Elf_AllDataBuffer[1024 * 1024];

int main(void)
{
	
	uint32_t ISR_Offest;
	
	FILE *Elf_File_Src;
	uint8_t * Elf_File_Src_Name = "Main_APP.elf";
	uint32_t Elf_File_Src_Size;	
	
	FILE * Elf_File_Output;
	uint8_t * Elf_File_Output_Name = "Elf_Parser_Output.txt";
	
	
	
	/* open the src file  */
	Elf_File_Src = fopen( Elf_File_Src_Name , "rb");
	
	/* calculate the size of the bytes */
	fseek(Elf_File_Src , 0 , SEEK_END);
	Elf_File_Src_Size = ftell(Elf_File_Src);
	fseek(Elf_File_Src , 0 , SEEK_SET);
 
	/* Collecting data */
	for(uint32_t iteration_index = 0 ; iteration_index < Elf_File_Src_Size ; iteration_index++)
	{
		Elf_AllDataBuffer[iteration_index] = fgetc (Elf_File_Src);
	}
	 
	 
	 /* open the output file  */
	Elf_File_Output = fopen( Elf_File_Output_Name , "w");
	
	Elf_Header_t *Elf_Header = (Elf_Header_t*)Elf_AllDataBuffer;

	/* Check if the file is ELF file */
	if( Elf_Header->identity[0] == 0x7F && Elf_Header->identity[1] == 'E' && Elf_Header->identity[2] == 'L' && Elf_Header->identity[3] == 'F' )
	{
		/* Write the first title */
		fprintf(Elf_File_Output ,"File Header Infomation: \n\n");
		
		/* The Magic Number Information */
		fprintf(Elf_File_Output ,"  %-30s : 0x%2X 0x%2X 0x%2X 0x%2X  (%s)\n", "Header Magic Nymber " , Elf_Header->identity[MAG_0] , Elf_Header->identity[MAG_1] ,\
			Elf_Header->identity[MAG_2] , Elf_Header->identity[MAG_3] , "[0x7F ELF]");
			
		/* The Class of the ELF File */
		switch(Elf_Header->identity[CLASS])
		{
			case CLASS_NONE:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF CLASS " , Elf_Header->identity[CLASS] , "[ CLASS - None ]" );
				break;
				
			case CLASS_32:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF CLASS " , Elf_Header->identity[CLASS] , "[ CLASS - ELF32 bits ]" );
				break;
				
			case CLASS_64:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF CLASS " , Elf_Header->identity[CLASS] , "[ CLASS - ELF64 bits ]" );
				break;
		}
		
		/* The encoding of the Data of The ELF File */
		switch(Elf_Header->identity[DATA])
		{
			case CLASS_NONE:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Data " , Elf_Header->identity[DATA] , "[ DATA - None ]" );
				break;
				
			case CLASS_32:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Data " , Elf_Header->identity[DATA] , "[ DATA - Little Endian ]" );
				break;
				
			case CLASS_64:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Data " , Elf_Header->identity[DATA] , "[ DATA - Big Endian ]" );
				break;
		}
		
		/* The Version of the ELF File */
		fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Version " , Elf_Header->identity[VERSION] , "[ Current ]" );
		
		/* The type of The ELF File */
		switch(Elf_Header->type)
		{
			case TYPE_NONE:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Typ: " , Elf_Header->type , "[ Type - None ]" );
				break;
				
			case TYPE_REL:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Type " , Elf_Header->type , "[ Type - Relocatable file ]" );
				break;
				
			case TYPE_EXEC:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Type " , Elf_Header->type , "[ Type - Executable file ]" );
				break;
				
			case TYPE_DYN:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Type " , Elf_Header->type , "[ Type - Shared object file ]" );
				break;
				
			case TYPE_CORE:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Type " , Elf_Header->type , "[ Type - Core file ]" );
				break;
		}
		
		/* the Machine "not completed - i printed only ARM otherwise the number will be printed only "*/
		
		switch(Elf_Header->machine)
		{
			case MACHINE_ARM:
					fprintf(Elf_File_Output ,"  %-30s : %-10d  (%s)\n", "ELF Machine " , Elf_Header->machine , "[ Machine - ARM ]" );
				break;
				
			default:
					fprintf(Elf_File_Output ,"  %-30s : %-10d \n", "ELF Machine " , Elf_Header->machine );
				break;
		}
		
		/* The Entry point */
		fprintf(Elf_File_Output ,"  %-30s : 0x%X \n", "ELF Entry: " , Elf_Header->entry );
		
		/* The Program Headers table Offest */
		fprintf(Elf_File_Output ,"  %-30s : %-10d (%s)\n", "ELF P_Header Table Offest" , Elf_Header->ph_offset , "[ Bytes from the beginning of the file ]" );
		
		/* The Section Headers Offest */
		fprintf(Elf_File_Output ,"  %-30s : %-10d (%s)\n", "ELF S_Header Table Offest" , Elf_Header->sh_offset , "[ Bytes from the beginning of the file ]" );
		
		/* The Flag of the Header File*/
		fprintf(Elf_File_Output ,"  %-30s : 0x%X\n", "ELF Flags " , Elf_Header->flags );
		
		/* The Header Size */
		fprintf(Elf_File_Output ,"  %-30s : %-10d (%s)\n", "ELF Header Size " , Elf_Header->header_size , "[ Bytes ]" );
			
		/* One Program  Size "all of them have the same size" */
		fprintf(Elf_File_Output ,"  %-30s : %-10d (%s)\n", "ELF One Program Size " , Elf_Header->ph_size , "[ Bytes ]" );
		
		/* Number of Programs sections */
		fprintf(Elf_File_Output ,"  %-30s : %-10d (%s)\n", "ELF Num of Programs " , Elf_Header->ph_num , "[ Bytes ]" );
		
		/* One Section Size "all of them have the same size" */
		fprintf(Elf_File_Output ,"  %-30s : %-10d (%s)\n", "ELF One Section Size " , Elf_Header->sh_size , "[ Bytes ]" );
		
		/* Number of Sections sections */
		fprintf(Elf_File_Output ,"  %-30s : %-10d (%s)\n", "ELF Num of Sections " , Elf_Header->sh_num , "[ Bytes ]" );
		
		/* Header TAble index */
		fprintf(Elf_File_Output ,"  %-30s : %-10d (%s)\n", "ELF S_Header string table IDX " , Elf_Header->strtab_index , "[ Bytes ]" );
		
		
		
		
		
		
		/* Program Headers Info */
		Elf_Program_Header_t *Elf_Program_Header = (Elf_Program_Header_t *) &Elf_AllDataBuffer[Elf_Header->ph_offset];
		
		
		/* Print the Title */
		fprintf(Elf_File_Output ,"\nProgram Headers INFO \n");
		fprintf(Elf_File_Output ,"  %-10s    %-10s    %-10s    %-10s    %-10s    %-10s    %-6s    %-10s\n",\
									"Type" , "Offest" , "VirAddr" , "PhysAddr" , "FileSize" , "MemSize" , "Flag" , "Align");


		for(uint8_t index=0 ; index<(Elf_Header->ph_num) ; index++)
		{
			/* Type of this segement */
			switch(Elf_Program_Header[index].type)
			{
				case TYPE_NULL	:
					fprintf(Elf_File_Output ,"  %-10s" , "NULL");
						break;
						
				case TYPE_LOAD  :
					fprintf(Elf_File_Output ,"  %-10s" , "LOAD");
						break;
						
				case TYPE_DYNAMIC:
					fprintf(Elf_File_Output ,"  %-10s" , "DYNAMIC");
						break;
						
				case TYPE_INTERP:
					fprintf(Elf_File_Output ,"  %-10s" , "INTERPRETER");
						break;
						
				case TYPE_NOTE  :
					fprintf(Elf_File_Output ,"  %-10s" , "AUXILIARY");
						break;
						
				case TYPE_SHLIB :
					fprintf(Elf_File_Output ,"  %-10s" , "RESERVED");
						break;
						
				case TYPE_PHDR  :
					fprintf(Elf_File_Output ,"  %-10s" , "PRG_HEADER");
						break;
						
				case TYPE_LOPROC:
					fprintf(Elf_File_Output ,"  %-10s" , "PROCERSSOR");
						break;
						
				case TYPE_HIPROC:
					fprintf(Elf_File_Output ,"  %-10s" , "PROCESSOR");
						break;
			}
			
			/* Offest */
			fprintf(Elf_File_Output ,"    0x%08X" ,Elf_Program_Header[index].offset );
			/* Virtual Address */
			fprintf(Elf_File_Output ,"    0x%08X" ,Elf_Program_Header[index].virtual_address );
			/* Physical Address */
			fprintf(Elf_File_Output ,"    0x%08X" ,Elf_Program_Header[index].physical_address );
			/* File size */
			fprintf(Elf_File_Output ,"    0x%08X" ,Elf_Program_Header[index].file_size );
			/* Memory Size */
			fprintf(Elf_File_Output ,"    0x%08X" ,Elf_Program_Header[index].mem_size );
			
			switch(Elf_Program_Header[index].flags)
			{
				case FLAG_NONE	:
					fprintf(Elf_File_Output ,"    %-6s" , "NONE");
						break;
				
				case FLAG_E	:
					fprintf(Elf_File_Output ,"    %-6s" , "E");
						break;
						
				case FLAG_W  :
					fprintf(Elf_File_Output ,"    %-6s" , "W");
						break;
						
				case FLAG_WE:
					fprintf(Elf_File_Output ,"    %-6s" , "WE");
						break;
						
				case FLAG_R:
					fprintf(Elf_File_Output ,"    %-6s" , "R");
						break;
						
				case FLAG_RE  :
					fprintf(Elf_File_Output ,"    %-6s" , "RE");
						break;
						
				case FLAG_RW :
					fprintf(Elf_File_Output ,"    %-6s" , "RW");
						break;
						
				case FLAG_RWE  :
					fprintf(Elf_File_Output ,"    %-6s" , "RWE");
						break;
			}
			
			/* Align */
			fprintf(Elf_File_Output ,"    0x%08X\n" ,Elf_Program_Header[index].align );
		}
		
		
		
		
		/* Sections */
		
		/* Program Headers Info */
		Elf_Section_Header_t *Elf_Section_Header = (Elf_Section_Header_t *) &Elf_AllDataBuffer[Elf_Header->sh_offset];
		
		ISR_Offest = Elf_Section_Header[1].sh_addr;
	}
	else
	{
		fprintf(Elf_File_Output ,"This is not an ELF file \n");
	}

	/* close the file */
	fclose(Elf_File_Src);
	fclose(Elf_File_Output);

	return 0;
}


