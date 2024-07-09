#pragma once
#define VGA_CRT 0              //CRT set
#define VGA_ACT 1              //attribute controller set
#define VGA_GEN 2              //general registers set
#define VGA_SEQ 3              //sequencer set
#define VGA_GCT 4              //graphics controller set
#define VGA_OLD 5              //old pallete enable

#define VGA__GEN__MIS 0        //miscellaneous in the generals set
#define VGA__GEN__ST1 1        //status 1 in the generals set, readonly
#define VGA__OLD__ENA 0        //old pallete enable index

#define VGA__GEN_MISC_R 0x3cc  //general miscellaneous, read
#define VGA__GEN_MISC_W 0x3c2  //general miscellaneous, write
#define VGA__GEN_STATUS1 0x3da //general status 1
#define VGA__SEQ_ADDR 0x3c4    //sequencer, address
#define VGA__SEQ_DATA 0x3c5    //sequencer, data
#define VGA__SEQ__RST 0        //sequencer, reset
#define VGA__SEQ__CLK 1        //sequencer, clock mode
#define VGA__SEQ__MAP 2        //sequencer, map selection
#define VGA__SEQ__FNT 3        //sequencer, font selection
#define VGA__SEQ__MEM 4        //sequencer, memory mode
#define VGA__CRT_ADDR 0x3d4    //CRT, address
#define VGA__CRT_DATA 0x3d5    //CRT, data
#define VGA__CRT__HTO 0        //CRT, horizontal total
#define VGA__CRT__HDE 1        //CRT, horizontal display-enable end
#define VGA__CRT__HBS 2        //CRT, horizontal blanking start
#define VGA__CRT__HBE 3        //CRT, horizontal blanking end
#define VGA__CRT__HRS 4        //CRT, horizontal retrace start
#define VGA__CRT__HRE 5        //CRT, horizontal retrace end
#define VGA__CRT__VTO 6        //CRT, vertical total
#define VGA__CRT__MSB 7        //CRT, most significant bits
#define VGA__CRT__PRS 8        //CRT, preset scanline
#define VGA__CRT__MSL 9        //CRT, maximum scanline
#define VGA__CRT__CSL 10       //CRT, cursor start
#define VGA__CRT__CEL 11       //CRT, cursor end
#define VGA__CRT__SAH 12       //CRT, start address (high)
#define VGA__CRT__SAL 13       //CRT, start address (low)
#define VGA__CRT__CLH 14       //CRT, cursor location (high)
#define VGA__CRT__CLL 15       //CRT, cursor location (low)
#define VGA__CRT__VRS 16       //CRT, vertical retrace start
#define VGA__CRT__VRE 17       //CRT, vertical retrace end
#define VGA__CRT__VDE 18       //CRT, vertical display-enable end
#define VGA__CRT__OFF 19       //CRT, offset (logical line width)
#define VGA__CRT__ULL 20       //CRT, underline location
#define VGA__CRT__VBS 21       //CRT, vertical blanking start
#define VGA__CRT__VBE 22       //CRT, vertical blanking end
#define VGA__CRT__MOD 23       //CRT, mode
#define VGA__CRT__SSL 24       //CRT, line compare (split screen line)
#define VGA__GCT_ADDR 0x3ce    //graphics controller, address
#define VGA__GCT_DATA 0x3cf    //graphics controller, data
#define VGA__GCT__WRV 0        //graphics controller, write value
#define VGA__GCT__WRS 1        //graphics controller, write selection
#define VGA__GCT__COC 2        //graphics controller, color compare
#define VGA__GCT__DRO 3        //graphics controller, data rotate
#define VGA__GCT__RDM 4        //graphics controller, read map
#define VGA__GCT__GMO 5        //graphics controller, graphics mode
#define VGA__GCT__MIS 6        //graphics controller, miscelaneous
#define VGA__GCT__CSE 7        //graphics controller, color selection
#define VGA__GCT__BIT 8        //graphics controller, bit mask
#define VGA__ACT_ADDA 0x3c0    //attribute controller, address and data
#define VGA__ACT_READ 0x3c1    //attribute controller, read
#define VGA__ACT__ATB 16       //attribute controller, attributes
#define VGA__ACT__BOR 17       //attribute controller, border color
#define VGA__ACT__PEN 18       //attribute controller, plane enable
#define VGA__ACT__OFF 19       //attribute controller, offset
#define VGA__ACT__CSE 20       //attribute controller, color selection
#define VGA__DAC_ADDR_R 0x3c7  //pallete, read address
#define VGA__DAC_ADDR_W 0x3c8  //pallete, write address
#define VGA__DAC_DATA 0x3c9    //pallete, data
#define VGA__DAC_MASK 0x3c6    //pallete, bit mask

//VGA register structure
typedef struct
{
char
gen_mis,
seq[5],
crt[25],
gct[9],
act[5];
}vgaregs_t;

//tables for horizontal configurations
//SEQ_CLK,SEQ_MEM,CRT_HTO,CRT_HDE,CRT_HBS,CRT_HBE,CRT_HRS,CRT_HRE,CRT_OFF,GCT_GMO,GCT_MIS,ACT_ATB
char tbl_hor_txt40[12]={0x08,0x00,0x2d,0x27,0x28,0x10,0x2b,0xa0,0x14,0x10,0x0e,0x00};
char tbl_hor_txt80[12]={0x00,0x00,0x5f,0x4f,0x50,0x02,0x55,0x81,0x28,0x10,0x0e,0x00};
char tbl_hor_txt90[12]={0x00,0x00,0x6b,0x59,0x5a,0x0e,0x5f,0xac,0x2d,0x10,0x0e,0x00};
                                                    //0x5e,0xaa
char tbl_hor_gra640[12]={0x01,0x04,0x5f,0x4f,0x50,0x02,0x54,0x80,0x28,0x00,0x05,0x01};
                                                     //0x55,0x81
char tbl_hor_gra720[12]={0x01,0x04,0x6b,0x59,0x5a,0x0e,0x5e,0xaa,0x2d,0x00,0x05,0x01};
                                                     //0x5f,0xac
char tbl_hor_gra320[12]={0x01,0x04,0x5f,0x4f,0x50,0x02,0x54,0x80,0x28,0x40,0x05,0x41};
                                                     //0x55,0x81
char tbl_hor_gra360[12]={0x01,0x04,0x6b,0x59,0x5a,0x0e,0x5e,0xaa,0x2d,0x40,0x05,0x41};
                                                     //0x5f,0xac

//tables for vertical configurations
//GEN_MIS,CRT_VTO,CRT_MSB,CRT_MSL,CRT_VRS,CRT_VRE,CRT_VDE,CRT_VBS,CRT_VBE
char tbl_ver_txt20_12[9]={0xc0,0x0b,0x3e,0xcb,0xea,0x0c,0xdf,0xe7,0x04};
char tbl_ver_txt25_8[9]={0x40,0xbf,0x1f,0xc7,0x9c,0x2e,0x8f,0x96,0xb9};
char tbl_ver_txt25_14[9]={0x80,0xbf,0x1f,0x4d,0x83,0x35,0x5d,0x63,0xba};
char tbl_ver_txt25_16[9]={0x40,0xbf,0x1f,0x4f,0x9c,0x2e,0x8f,0x96,0xb9};
char tbl_ver_txt30_8[9]={0xc0,0x0b,0x3e,0xc7,0xea,0x0c,0xdf,0xe7,0x04};
char tbl_ver_txt30_16[9]={0xc0,0x0b,0x3e,0x4f,0xea,0x0c,0xdf,0xe7,0x04};
char tbl_ver_txt40_12[9]={0xc0,0x0b,0x3e,0x4b,0xea,0x0c,0xdf,0xe7,0x04};
char tbl_ver_txt50_8[9]={0x40,0xbf,0x1f,0x47,0x9c,0x2e,0x8f,0x96,0xb9};
char tbl_ver_txt60_8[9]={0xc0,0x0b,0x3e,0x47,0xea,0x0c,0xdf,0xe7,0x04};
char tbl_ver_gra200[9]={0x40,0xbf,0x1f,0x41,0x9c,0x2e,0x8f,0x96,0xb9};
char tbl_ver_gra240[9]={0xc0,0x0b,0x3e,0x41,0xea,0x0c,0xdf,0xe7,0x04};
char tbl_ver_gra350[9]={0x80,0xbf,0x1f,0x40,0x83,0x35,0x5d,0x63,0xba};
char tbl_ver_gra400[9]={0x40,0xbf,0x1f,0x40,0x9c,0x2e,0x8f,0x96,0xb9};
char tbl_ver_gra480[9]={0xc0,0x0b,0x3e,0x40,0xea,0x0c,0xdf,0xe7,0x04};

//tables for video modes
//SEQ_CLK,ACT_OFF,CRT_MOD
char tbl_mod_txt8p[]={0x01,0x00,0x00};
char tbl_mod_txt9p[]={0x00,0x08,0x00};
char tbl_mod_gra16c[]={0x01,0x00,0x40};
char tbl_mod_gra256c[]={0x01,0x00,0x40};

//functions

//writes a value in a VGA register
//set is 0 for CRT controller, 1 for attribute controller, 2 for
//generals, 3 for sequencer, 4 for graphics controller, 5 for
//old pallete enable
void VGAWriteReg(char set, char index, char value)
{
	switch(set)
	{
		case VGA_CRT:
			if(index<=24)
			{
				outb(VGA__CRT_ADDR,index);
				outb(VGA__CRT_DATA,value);
			}
			break;
		case VGA_ACT:
			if(index<=20)
			{
				inb(VGA__GEN_STATUS1);
				index=(inb(VGA__ACT_ADDA)&0x20)|index;
				inb(VGA__GEN_STATUS1);
				outb(VGA__ACT_ADDA,index);
				outb(VGA__ACT_ADDA,value);
			}
			break;
		case VGA_GEN:
			if(index==0)
			{
				outb(VGA__GEN_MISC_W,value);
			}
			break;
		case VGA_SEQ:
			if(index<=4)
			{
				outb(VGA__SEQ_ADDR,index);
				outb(VGA__SEQ_DATA,value);
			}
			break;
		case VGA_GCT:
			if(index<=8)
			{
				outb(VGA__GCT_ADDR,index);
				outb(VGA__GCT_DATA,value);
			}
			break;
		case VGA_OLD:
			if(index==0)
			{
				value=value?0x20:0;
				inb(VGA__GEN_STATUS1);
				outb(VGA__ACT_ADDA,value);
			}
			break;
	}
}

//reads a value from a VGA register
//set is 0 for CRT controller, 1 for attribute controller, 2 for
//generals, 3 for sequencer, 4 for graphics controller, 5 for
//old pallete enable
char VGAReadReg(char set, char index)
{
	char r=0;
	switch(set)
	{
	case VGA_CRT:
		if(index<=24)
		{
			outb(VGA__CRT_ADDR,index);
			r=inb(VGA__CRT_DATA);
		}
		break;
	case VGA_ACT:
		if(index<=20)
		{
			inb(VGA__GEN_STATUS1);
			index=(inb(VGA__ACT_ADDA)&0x20)|index;
			inb(VGA__GEN_STATUS1);
			outb(VGA__ACT_ADDA,index);
			r=inb(VGA__ACT_READ);
		}
		break;
	case VGA_GEN:
		if(index==VGA__GEN__MIS)
		{
			r=inb(VGA__GEN_MISC_R);
		}
		if(index==VGA__GEN__ST1)
		{
			r=inb(VGA__GEN_STATUS1);
		}
		break;
	case VGA_SEQ:
		if(index<=4)
		{
			outb(VGA__SEQ_ADDR,index);
			r=inb(VGA__SEQ_DATA);
		}
		break;
	case VGA_GCT:
		if(index<=8)
		{
			outb(VGA__GCT_ADDR,index);
			r=inb(VGA__GCT_DATA);
		}
		break;
	case VGA_OLD:
		if(index==0)
		{
			inb(VGA__GEN_STATUS1);
			r=(inb(VGA__ACT_ADDA)>>5)&1;
		}
		break;
	}
return(r);
}

//resets the registers in a VGA register structure
void resetregs(vgaregs_t*regs)
{
	long unsigned int i;
	char*p=(char*)regs;
	for(i=0;i<sizeof(vgaregs_t);i++)
	{
		*p++=0;
	}
	regs->gen_mis=0x23;//enable video memory, normal mode, upper 64KB page
	regs->seq[VGA__SEQ__CLK]=0x02;//set bit to 1
	regs->seq[VGA__SEQ__MAP]=0x0f;//select the 4 planes
	regs->seq[VGA__SEQ__MEM]=0x02;//enable 256KB
	regs->crt[VGA__CRT__HBE]=0x80;//set bit to 1
	regs->crt[VGA__CRT__VRE]=0x30;//disable IRQs
	regs->crt[VGA__CRT__ULL]=31;//underline location
	regs->crt[VGA__CRT__MSB]=0x10;//split screen
	regs->crt[VGA__CRT__MSL]=0x40;
	regs->crt[VGA__CRT__SSL]=0xff;
	regs->crt[VGA__CRT__MOD]=0xa3;//normal mode
	regs->gct[VGA__GCT__BIT]=0xff;//bit mask
	regs->act[VGA__ACT__ATB-16]=0x04;//enable line characters
	regs->act[VGA__ACT__PEN-16]=0x0f;//enable the 4 planes
}

//changes the video mode by writing in all the VGA registers
void vgachangemode(vgaregs_t*regs)
{
	short i;
	//unlock some CRTC registrrs
	VGAWriteReg(VGA_CRT,VGA__CRT__VRE,VGAReadReg(VGA_CRT,VGA__CRT__VRE)&0x7f);
	//enable pallete
	outb(VGA__DAC_MASK,0xff);
	//wait until display-enable start
	//while(!(VGAReadReg(VGA_GEN,VGA__GEN__ST1)&0x08));
	//while(VGAReadReg(VGA_GEN,VGA__GEN__ST1)&0x08);
	//stop sequencer
	VGAWriteReg(VGA_SEQ,VGA__SEQ__RST,0x01);
	//write in the registers
	VGAWriteReg(VGA_GEN,VGA__GEN__MIS,regs->gen_mis);
	for(i=1;i<5;i++)
		VGAWriteReg(VGA_SEQ,i,regs->seq[i]);
	for(i=0;i<25;i++)
		VGAWriteReg(VGA_CRT,i,regs->crt[i]);
	for(i=0;i<9;i++)
		VGAWriteReg(VGA_GCT,i,regs->gct[i]);
	for(i=0;i<5;i++)
		VGAWriteReg(VGA_ACT,i+16,regs->act[i]);
	//reset old pallete
	VGAWriteReg(VGA_OLD,VGA__OLD__ENA,0);
	for(i=0;i<16;i++)
		VGAWriteReg(VGA_ACT,i,i);
	VGAWriteReg(VGA_OLD,VGA__OLD__ENA,1);
	//resume sequencer
	VGAWriteReg(VGA_SEQ,VGA__SEQ__RST,0x03);
}

//selects the text plane
void settextplane(void)
{
	VGAWriteReg(VGA_GCT,VGA__GCT__GMO,0x10);
	VGAWriteReg(VGA_GCT,VGA__GCT__MIS,(VGAReadReg(VGA_GCT,VGA__GCT__MIS)&0x0c)|2);
	VGAWriteReg(VGA_SEQ,VGA__SEQ__MAP,0x03);
	VGAWriteReg(VGA_GCT,VGA__GCT__RDM,1);
	VGAWriteReg(VGA_SEQ,VGA__SEQ__MEM,0x02);
}

//selects the font plane
void setfontplane(void)
{
	VGAWriteReg(VGA_GCT,VGA__GCT__GMO,0x00);
	VGAWriteReg(VGA_GCT,VGA__GCT__MIS,VGAReadReg(VGA_GCT,VGA__GCT__MIS)&0x0c);
	VGAWriteReg(VGA_SEQ,VGA__SEQ__MAP,0x04);
	VGAWriteReg(VGA_GCT,VGA__GCT__RDM,2);
	VGAWriteReg(VGA_SEQ,VGA__SEQ__MEM,0x06);
}

//modifies the VGA pallete
//pal=pointer to the first pallete color to be used
//first=first color to modify
//num=number of colors to modify
void VGASetPal(char*pal,char first,short num)
{
	if((num+first)>256)num=256-first;
	if(!num)return;
	num*=3;
	outb(VGA__DAC_ADDR_W,first);
	while(num--)
	{
		outb(VGA__DAC_DATA,(*pal));
		pal++;
	}
}

//obtains the VGA pallete
//pal=pointer to the first pallete color to be overwritten
//first=first color to read
//num=number of colors to read
void VGAGetPal(char*pal,char first,short num)
{
	if((num+first)>256)num=256-first;
	if(!num)return;
	num*=3;
	outb(VGA__DAC_ADDR_R,first);
	while(num--)
	{
		*pal=inb(VGA__DAC_DATA)<<2;
		pal++;
	}
}

//puts the text cursor in the x,y coordinates
//w is the visual screen width in characters
void VGAGotoXY(char x,char y,char w)
{
	short c;
	c=y;c*=w;
	c+=x;
	VGAWriteReg(VGA_CRT,VGA__CRT__CLH,c>>8);
	VGAWriteReg(VGA_CRT,VGA__CRT__CLL,c);
}

//gets the cursor coordinates and returns them in x,y
//w is the visual screen width in characters
void VGAWhereXY(char*x,char*y,char w)
{
	short c;
	c=VGAReadReg(VGA_CRT,VGA__CRT__CLH);
	c<<=8;
	c|=VGAReadReg(VGA_CRT,VGA__CRT__CLL);
	*x=c%w;
	*y=c/w;
}

//changes the text cursor appearence
//y=first line
//h=block height
void VGASetCursor(char y,char h)
{
	VGAWriteReg(VGA_CRT,VGA__CRT__CSL,y&31);
	VGAWriteReg(VGA_CRT,VGA__CRT__CEL,(y+h-1)&31);
}

//gets the text cursor appearence
//y=first line
//h=block height
void VGAGetCursor(char*y,char*h)
{
	*y=VGAReadReg(VGA_CRT,VGA__CRT__CSL)&31;
	*h=VGAReadReg(VGA_CRT,VGA__CRT__CEL)&31;
	*h-=*y;
	(*h)++;
}

//enables or disables the text cursor
void VGACursor(char e)
{
	e=e?0:0x20;
	VGAWriteReg(VGA_CRT,VGA__CRT__CSL,(VGAReadReg(VGA_CRT,VGA__CRT__CSL)&31)|e);
}/*end function*/

//modifies the text font
//fnt=pointer to the first font character to be used
//ch=character height in the font
//first=first character in the VGA font to modify
//num=number of characters to modify
void VGASetFont(char*fnt,char ch,short first,short num)
{
	char*v=(char*)0xb8000;
	short i,j;
	if((num+first)>512)num=512-first;
	if(!num)return;
	v+=first*32;
	setfontplane();
	for(i=0;i<num;i++)
	{
		for(j=0;j<ch;j++)
		{
			v[j]=fnt[j];
		}
		for(j=ch;j<32;j++)
		{
			v[j]=0;
		}
		fnt+=ch;
		v+=32;
	}
	settextplane();
}

//obtains the text font
//fnt=pointer to the first font character to be overwritten
//ch=character height in the font
//first=first character in the VGA font to read
//num=number of characters to read
void VGAGetFont(char*fnt,char ch,short first,short num)
{
	char*v=(char*)0xb8000;
	short i,j;
	if((num+first)>512)num=512-first;
	if(!num)return;
	v+=first*32;
	setfontplane();
	for(i=0;i<num;i++)
	{
		for(j=0;j<ch;j++)
		{
			fnt[j]=v[j];
		}
		fnt+=ch;
		v+=32;
	}
	settextplane();
}

//waits until a vertical retrace period starts
void VGAWaitVBL(void)
{
	while(VGAReadReg(VGA_GEN,VGA__GEN__ST1)&8);
	while(!(VGAReadReg(VGA_GEN,VGA__GEN__ST1)&8));
}/*end function*/

//sets a VGA video mode
//returns 1 if the mode is valid, 0 if not
//m=mode
//  0=text (8-pixels-wide characters)
//  1=text (9-pixels-wide characters)
//  2=graphics 16 colors
//  3=graphics 256 colors
//w=width (in characters or in pixels)
//h=height (in characters or in pixels)
//o=option
//  text modes:
//    8=8-pixels-high characters
//    12=12-pixels-high characters
//    14=14-pixels-high characters
//    16=16-pixels-high characters
//  graphic modes:
//    0=normal
//    1=chained mode
//----------------------------------------------------------------
//text mode widths (8p): 40, 80, 90
//text mode widths (9p): 40, 80
//text mode heights (8-pix-h): 25, 30, 50, 60
//text mode heights (12-pix-h): 20, 40
//text mode heights (14-pix-h): 25
//text mode heights (16-pix-h): 25, 30
//graphic mode widths (16c): 640, 720
//graphic mode widths (256c): 320, 360
//graphic mode heights: 200, 240, 350, 400, 480
//chained mode only allows 320*200 at 256 colors
char VGAMode(char m,short w,short h,char o)
{
	char*tbl_hor,*tbl_ver,*tbl_mod;
	char*v;
	int i;
	vgaregs_t regs;
	char chained=0;
	char curstart=0,curheight=0;
	resetregs(&regs);
	switch(m)
	{
		case 0://txt 8p
		case 1://txt 9p
			if(m)
			{
				tbl_mod=tbl_mod_txt9p;
				regs.gen_mis|=0x04;
			}
			else
			{
				tbl_mod=tbl_mod_txt8p;
			}
			switch(w)
			{
				case 40:
					tbl_hor=tbl_hor_txt40;
					break;
				case 80:
					tbl_hor=tbl_hor_txt80;
					break;
				case 90:
					if(m==1)return(0);
					tbl_hor=tbl_hor_txt90;
					regs.gen_mis|=0x04;
					break;
				default:
					return(0);
			}
			switch(h)
			{
				case 20:
					switch(o)
					{
						case 12:
							tbl_ver=tbl_ver_txt20_12;
							break;
						default:
							return(0);
					}
					break;
				case 25:
					switch(o)
					{
						case 8:
							tbl_ver=tbl_ver_txt25_8;
							break;
						case 14:
							tbl_ver=tbl_ver_txt25_14;
							break;
						case 16:
							tbl_ver=tbl_ver_txt25_16;
							break;
						default:
							return(0);
					}
					break;
				case 30:
					switch(o)
					{
						case 8:
							tbl_ver=tbl_ver_txt30_8;
							break;
						case 16:
							tbl_ver=tbl_ver_txt30_16;
							break;
						default:
							return(0);
					}
					break;
				case 40:
					switch(o)
					{
						case 12:
							tbl_ver=tbl_ver_txt40_12;
							break;
						default:
							return(0);
					}
					break;
				case 50:
					switch(o)
					{
						case 8:
							tbl_ver=tbl_ver_txt50_8;
							break;
						default:
							return(0);
					}
					break;
				case 60:
					switch(o)
					{
						case 8:
							tbl_ver=tbl_ver_txt60_8;
							break;
						default:
							return(0);
					}
					break;
				default:
					return(0);
			}
			break;
		case 2://gra 16c
			if(o)return(0);
			tbl_mod=tbl_mod_gra16c;
			switch(w)
			{
				case 640:
					tbl_hor=tbl_hor_gra640;
					break;
				case 720:
					regs.gen_mis|=0x04;
					tbl_hor=tbl_hor_gra720;
					break;
				default:
					return(0);
			}
			switch(h)
			{
				case 200:
					tbl_ver=tbl_ver_gra200;
					break;
				case 240:
					tbl_ver=tbl_ver_gra240;
					break;
				case 350:
					tbl_ver=tbl_ver_gra350;
					break;
				case 400:
					tbl_ver=tbl_ver_gra400;
					break;
				case 480:
					tbl_ver=tbl_ver_gra480;
					break;
				default:
					return(0);
			}
			break;
		case 3://gra 256c
			tbl_mod=tbl_mod_gra256c;
			switch(w)
			{
				case 320:
					tbl_hor=tbl_hor_gra320;
					break;
				case 360:
					regs.gen_mis|=0x04;
					tbl_hor=tbl_hor_gra360;
					break;
				default:
					return(0);
			}
			switch(h)
			{
				case 200:
					tbl_ver=tbl_ver_gra200;
					break;
				case 240:
					tbl_ver=tbl_ver_gra240;
					break;
				case 350:
					tbl_ver=tbl_ver_gra350;
					break;
				case 400:
					tbl_ver=tbl_ver_gra400;
					break;
				case 480:
					tbl_ver=tbl_ver_gra480;
					break;
				default:
					return(0);
			}
			switch(o)
			{
				case 0:
					break;
				case 1:
					chained=1;
					if((w!=320)||(h!=200))
						return(0);
					break;
				default:
					return(0);
			}
			break;
		default:
			return(0);
	}
	regs.seq[VGA__SEQ__CLK]|=tbl_hor[0];
	regs.seq[VGA__SEQ__MEM]|=tbl_hor[1];
	regs.crt[VGA__CRT__HTO]|=tbl_hor[2];
	regs.crt[VGA__CRT__HDE]|=tbl_hor[3];
	regs.crt[VGA__CRT__HBS]|=tbl_hor[4];
	regs.crt[VGA__CRT__HBE]|=tbl_hor[5];
	regs.crt[VGA__CRT__HRS]|=tbl_hor[6];
	regs.crt[VGA__CRT__HRE]|=tbl_hor[7];
	regs.crt[VGA__CRT__OFF]|=tbl_hor[8];
	regs.gct[VGA__GCT__GMO]|=tbl_hor[9];
	regs.gct[VGA__GCT__MIS]|=tbl_hor[10];
	regs.act[VGA__ACT__ATB-16]|=tbl_hor[11];

	regs.gen_mis|=tbl_ver[0];
	regs.crt[VGA__CRT__VTO]|=tbl_ver[1];
	regs.crt[VGA__CRT__MSB]|=tbl_ver[2];
	regs.crt[VGA__CRT__MSL]|=tbl_ver[3];
	regs.crt[VGA__CRT__VRS]|=tbl_ver[4];
	regs.crt[VGA__CRT__VRE]|=tbl_ver[5];
	regs.crt[VGA__CRT__VDE]|=tbl_ver[6];
	regs.crt[VGA__CRT__VBS]|=tbl_ver[7];
	regs.crt[VGA__CRT__VBE]|=tbl_ver[8];

	regs.seq[VGA__SEQ__CLK]|=tbl_mod[0];
	regs.act[VGA__ACT__OFF-16]|=tbl_mod[1];
	regs.crt[VGA__CRT__MOD]|=tbl_mod[2];

	if(chained)
	{
		regs.seq[VGA__SEQ__MEM]|=0x08;
		regs.crt[VGA__CRT__ULL]|=0x40;
		regs.crt[VGA__CRT__MOD]&=0xbf;
	}

	vgachangemode(&regs);
	switch(m)
	{
		case 0:
		case 1:
			setfontplane();
			v=(char*)0xb8000;
			for(i=0;i<32768;i++)*v++=0;
			settextplane();
			v=(char*)0xb8000;
			for(i=0;i<32768;i++)*v++=0;
			switch(o)
			{
				case 8:
					curstart=o-2;
					curheight=2;
					break;
				case 12:
					curstart=o-4;
					curheight=2;
					break;
				case 14:
					curstart=o-4;
					curheight=2;
					break;
				case 16:
					curstart=o-6;
					curheight=2;
			}
			VGASetCursor(curstart,curheight);
			break;
		case 2:
		case 3:
			v=(char*)0xa0000;
			VGAWriteReg(VGA_SEQ,VGA__SEQ__MAP,15);
			for(i=0;i<65536;i++)*v++=0;
	}
	return(1);
}