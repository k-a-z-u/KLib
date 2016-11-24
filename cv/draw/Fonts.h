#ifndef FONTS_H
#define FONTS_H

#include "../ImageFactory.h"
#include "../ImageChannel.h"

namespace K {

	const uint8_t FONT1_PNG[] = {
		137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,0,0,0,1,0,8,6,0,0,0,92,114,168,
		102,0,0,0,6,98,75,71,68,0,255,0,255,0,255,160,189,167,147,0,0,0,9,112,72,89,115,0,0,11,19,0,
		0,11,19,1,0,154,156,24,0,0,0,7,116,73,77,69,7,224,6,22,12,44,16,13,62,206,111,0,0,0,25,116,
		69,88,116,67,111,109,109,101,110,116,0,67,114,101,97,116,101,100,32,119,105,116,104,32,71,73,77,80,87,129,14,23,
		0,0,32,0,73,68,65,84,120,218,237,93,59,150,220,184,14,69,207,241,2,52,75,224,236,64,94,130,38,155,176,188,
		4,205,14,244,194,23,42,116,58,153,82,135,74,39,147,151,32,239,64,189,4,122,7,124,193,27,104,80,108,125,8,144,
		250,149,128,115,120,220,221,46,20,255,151,32,136,207,27,0,56,80,82,82,186,37,125,122,229,206,57,247,127,108,123,123,
		123,187,36,255,145,99,54,71,87,234,139,210,58,253,162,67,160,164,164,18,128,146,210,203,74,128,18,9,198,231,61,82,
		242,161,109,9,105,199,156,20,55,197,171,0,160,244,178,132,11,126,237,90,179,118,117,115,206,129,115,110,87,16,224,182,
		89,175,0,74,31,22,16,45,210,13,116,245,59,255,94,27,105,107,16,139,229,153,251,30,5,128,155,136,190,220,141,64,
		23,204,221,20,127,254,201,127,228,24,108,13,96,159,246,110,252,218,64,198,220,219,82,220,161,142,62,49,84,11,159,126,
		44,165,167,232,25,94,113,36,215,152,185,3,224,48,29,0,103,0,233,103,99,55,99,42,241,233,236,34,159,210,182,32,
		124,181,231,92,10,94,122,5,184,33,113,238,128,33,155,224,138,247,232,152,54,251,186,143,87,208,133,28,114,5,80,186,
		182,36,113,85,5,26,61,177,143,208,224,31,13,102,156,43,128,74,0,74,74,47,36,189,168,4,160,164,210,15,57,249,
		175,116,119,79,37,205,113,62,175,0,160,164,87,160,13,191,227,236,245,234,21,64,73,233,206,64,9,234,14,172,164,116,
		91,82,9,64,73,73,1,96,154,98,236,200,83,208,209,245,95,189,253,58,127,74,107,244,105,207,197,0,32,115,199,148,
		188,227,250,111,193,156,186,253,250,199,251,210,205,172,244,94,69,139,174,116,240,21,32,149,33,195,222,11,255,206,116,117,
		75,64,165,147,0,128,100,241,220,201,20,243,236,155,95,65,64,175,0,122,146,222,140,124,15,52,5,95,149,0,116,243,
		43,29,38,141,72,215,146,42,33,5,18,192,154,248,23,19,147,76,197,231,248,120,4,103,143,103,176,214,38,137,89,171,
		196,161,71,165,23,33,0,76,249,227,75,39,237,202,34,112,76,24,173,20,226,183,116,252,83,198,83,72,125,165,136,149,
		30,67,65,96,73,129,169,128,176,147,14,224,206,32,160,148,22,48,184,64,120,150,104,62,183,214,1,28,61,248,244,229,
		64,95,17,174,127,165,146,122,196,41,136,31,36,1,196,156,164,87,15,201,164,116,142,67,68,215,203,193,18,128,116,34,
		82,133,180,66,224,80,109,176,146,210,129,0,32,17,195,239,28,150,90,73,233,101,174,0,103,16,255,164,223,115,119,224,
		81,224,189,193,28,131,198,3,80,82,210,43,128,146,146,146,2,128,146,146,146,2,128,18,128,190,33,43,41,0,68,111,
		32,201,38,74,245,132,103,173,157,252,121,141,170,170,26,63,63,12,3,84,85,21,204,91,150,37,56,231,160,44,203,241,
		111,77,211,64,81,20,193,223,97,140,129,174,235,198,190,119,93,7,198,152,221,198,159,82,81,20,48,12,131,136,119,24,
		6,86,191,21,132,211,83,150,101,171,243,231,182,42,72,49,124,210,239,200,243,220,117,93,247,225,231,181,82,150,165,155,
		162,170,170,130,248,173,181,46,203,50,103,173,29,255,54,12,3,171,237,195,48,56,231,156,51,198,56,99,140,115,206,177,
		191,35,102,236,252,182,52,77,35,226,109,154,70,212,110,0,112,93,215,185,182,109,221,48,12,174,174,107,215,182,237,33,
		253,191,122,41,203,210,213,117,189,244,153,243,1,128,181,118,220,64,244,103,78,169,170,106,236,56,253,57,164,110,231,156,
		203,243,220,57,231,92,81,20,172,13,72,129,11,39,32,20,60,166,218,128,0,32,25,3,233,216,81,16,117,206,185,199,
		227,33,226,127,60,30,99,63,164,32,224,156,11,6,111,5,128,233,49,92,25,255,237,42,31,134,65,116,2,116,93,55,
		78,58,253,153,59,241,62,113,78,95,92,252,146,77,71,37,128,97,24,92,150,101,162,141,131,27,88,186,9,37,99,231,
		159,224,49,27,24,199,80,34,65,180,109,235,154,166,113,121,158,171,4,16,81,2,14,128,109,209,39,102,1,198,130,143,
		68,4,47,138,98,220,116,206,57,87,215,53,107,3,227,21,162,44,75,151,231,185,104,225,250,109,144,110,192,216,210,247,
		125,244,6,114,206,185,190,239,119,107,243,26,221,105,243,63,30,143,16,240,125,61,0,200,178,108,220,244,244,103,14,63,
		110,228,152,19,184,105,26,87,20,197,19,40,112,78,127,148,70,112,18,171,170,98,75,19,71,92,225,142,60,133,207,4,
		0,71,131,79,211,52,33,235,118,187,6,84,85,197,190,255,110,185,8,36,223,19,179,136,17,120,166,20,131,33,247,127,
		228,193,83,116,111,105,224,138,0,112,166,43,192,209,0,16,168,255,121,61,209,135,2,15,23,132,240,212,69,229,159,84,
		9,71,149,127,190,98,144,179,104,81,34,144,42,2,21,0,238,169,3,96,188,124,189,222,4,80,205,103,128,22,244,3,
		120,76,17,247,10,64,149,127,28,9,160,174,235,167,250,124,0,216,83,2,64,45,124,236,26,56,74,15,20,187,254,206,
		34,65,72,120,235,186,14,186,114,190,36,0,208,141,38,125,66,68,209,187,235,58,103,140,97,163,47,85,254,113,116,0,
		88,63,74,34,216,134,178,44,93,223,247,226,55,121,233,34,74,241,10,16,250,4,123,182,114,101,0,96,188,62,221,247,
		137,36,84,7,112,215,130,215,159,88,59,0,46,128,158,169,255,82,67,166,171,20,245,5,88,160,187,251,195,191,191,191,
		195,251,251,59,124,254,252,89,196,255,249,243,103,248,241,227,7,188,191,191,95,178,255,77,211,192,151,47,95,94,126,158,
		245,180,215,178,40,198,75,79,193,97,24,14,179,97,208,18,86,52,32,136,146,210,141,73,175,0,74,74,10,0,74,74,
		74,10,0,255,208,227,241,128,97,24,192,57,7,195,48,192,227,241,16,125,121,219,182,34,127,110,244,65,119,206,129,181,
		22,154,166,97,241,87,85,21,197,143,148,101,25,88,107,69,121,13,166,74,40,249,241,0,234,186,142,174,155,83,127,211,
		52,99,191,165,227,143,252,125,223,71,199,4,80,226,239,159,190,239,199,121,95,155,131,201,183,91,159,56,202,28,99,204,
		104,68,194,125,74,67,11,60,159,66,223,191,231,12,121,36,198,40,104,126,203,233,195,220,248,133,126,135,49,230,201,17,
		136,58,37,1,227,237,88,58,126,232,1,40,229,71,219,129,152,245,3,39,84,132,238,101,204,196,53,92,131,153,167,87,
		134,49,219,71,55,76,58,225,184,32,56,94,109,212,118,157,11,0,8,28,104,70,75,1,129,195,95,20,197,135,13,9,
		76,99,32,106,140,19,202,135,70,63,82,31,8,28,111,28,127,108,191,116,1,162,81,19,48,141,79,112,17,114,199,15,
		9,223,254,145,255,8,119,222,88,126,201,65,22,235,136,68,15,44,137,253,4,30,30,212,232,12,231,96,102,29,76,127,
		129,223,40,206,34,194,197,26,3,0,126,253,210,167,40,201,2,68,30,9,120,224,6,70,240,176,214,138,124,17,82,156,
		38,89,150,177,193,136,198,67,160,99,193,9,136,50,197,47,177,200,60,10,0,178,44,123,146,132,48,46,193,30,0,144,
		231,249,135,186,99,61,64,81,42,158,153,195,245,65,75,17,218,75,90,112,48,36,214,104,184,152,37,33,185,80,228,230,
		246,129,158,24,146,144,98,184,89,176,223,214,90,177,41,45,126,7,8,172,223,40,13,195,16,124,26,205,93,1,174,98,
		85,73,199,125,111,247,107,31,132,234,186,30,15,100,169,9,56,74,226,125,223,207,205,225,121,1,0,39,35,198,165,24,
		175,16,161,18,64,211,52,79,1,44,36,0,64,157,118,240,74,192,61,65,165,58,0,95,154,227,138,222,121,158,127,208,
		65,112,157,144,252,133,59,37,85,94,1,0,36,167,111,170,88,4,41,0,0,15,163,21,222,244,87,128,20,0,128,141,
		15,117,160,241,55,160,180,29,91,4,147,224,240,211,120,0,49,58,12,84,6,113,199,47,246,10,48,215,255,43,217,212,
		191,202,21,32,176,222,244,74,192,88,0,64,177,69,34,246,251,237,167,17,118,246,152,64,26,79,128,214,31,186,1,176,
		253,184,224,208,33,135,59,142,254,247,236,165,4,68,224,70,126,95,169,121,165,146,101,153,248,53,43,246,176,232,186,110,
		175,235,71,216,51,156,228,105,66,50,112,115,207,80,28,4,141,185,131,199,246,97,238,25,18,1,65,218,126,238,21,64,
		170,76,220,226,25,208,90,43,210,104,159,197,159,255,74,207,128,209,18,0,158,90,84,129,38,117,7,149,76,192,212,27,
		56,247,123,138,162,120,106,63,87,12,142,237,3,141,39,48,12,67,240,230,167,237,71,41,136,251,138,144,98,241,211,39,
		80,137,18,146,130,72,219,182,98,119,224,171,7,244,56,186,132,244,95,157,129,148,94,150,208,162,242,183,223,126,211,193,
		224,152,2,43,41,189,2,221,197,159,63,134,84,2,80,82,82,9,64,73,73,73,1,64,73,73,73,1,64,73,73,233,
		166,0,144,101,25,148,101,249,228,143,142,62,197,117,93,67,150,101,193,95,142,188,214,90,54,143,95,218,182,101,117,140,
		250,180,115,124,210,231,234,54,198,68,247,33,212,39,63,150,159,198,67,144,246,63,244,239,146,239,226,146,100,236,252,53,
		109,173,5,107,109,208,60,74,219,77,231,201,143,161,241,120,60,130,230,113,142,159,211,38,110,60,135,15,239,255,117,93,
		127,48,70,48,198,184,186,174,131,237,2,124,131,162,208,183,240,37,10,53,198,160,174,200,220,54,204,17,38,247,128,29,
		172,9,99,248,231,250,206,237,63,36,120,83,79,245,14,47,25,59,127,76,56,198,72,177,190,47,83,150,179,104,153,185,
		246,221,212,1,75,210,166,185,249,95,72,208,250,239,230,239,251,222,101,89,230,178,44,27,27,140,27,158,6,21,8,113,
		14,65,99,144,64,135,132,197,142,114,204,97,209,18,15,251,66,7,63,196,28,215,175,135,154,131,198,244,97,143,141,67,
		29,89,112,126,178,44,27,23,133,164,255,87,7,0,238,230,79,1,0,115,46,237,33,102,197,115,214,171,33,109,162,107,
		31,251,107,140,25,231,127,198,160,12,198,180,85,184,97,124,20,161,214,121,8,22,107,190,1,200,131,62,233,92,91,124,
		152,48,143,13,145,0,176,237,116,194,113,19,135,152,88,78,213,143,125,224,122,244,237,13,0,190,35,79,170,254,95,21,
		0,112,45,112,45,49,99,1,0,15,76,63,189,27,61,72,67,36,0,186,39,67,218,52,55,255,43,14,93,255,63,57,
		104,50,77,42,50,80,219,116,186,1,151,26,227,187,224,114,28,83,150,196,255,16,17,124,171,205,119,68,84,24,46,111,
		202,100,158,41,60,34,143,4,0,122,136,113,29,145,82,184,191,79,173,255,144,239,166,215,53,250,29,33,109,90,250,204,
		194,255,193,83,192,135,169,19,116,202,33,101,169,49,115,9,46,67,108,202,151,136,195,175,0,32,75,81,253,42,0,128,
		98,63,158,138,28,207,186,20,0,128,155,30,165,199,208,141,76,255,159,122,86,110,10,0,244,63,252,15,210,202,113,67,
		227,189,98,77,20,241,73,122,7,229,184,164,98,221,119,190,2,80,145,55,149,20,177,23,0,196,6,164,241,215,42,30,
		62,28,41,32,5,0,96,189,184,137,177,61,28,0,192,117,23,26,95,115,106,254,169,68,62,123,5,152,2,0,220,44,
		180,114,220,84,93,215,205,222,171,168,194,110,138,214,148,49,177,0,128,210,71,42,37,32,8,124,218,207,164,4,228,142,
		223,209,0,128,139,24,61,56,113,241,198,232,144,230,238,198,91,2,128,15,190,161,223,237,255,191,239,94,29,170,4,164,
		241,28,86,149,128,116,67,175,249,227,55,77,179,184,17,176,193,190,11,46,134,198,90,19,227,151,40,116,3,206,185,20,
		95,237,25,80,2,30,115,207,64,214,218,32,183,232,163,1,96,110,253,113,195,162,79,157,128,161,207,200,210,249,243,63,
		131,7,15,85,152,115,1,192,95,207,155,60,3,62,30,143,167,193,65,177,165,235,186,241,217,15,127,95,219,68,83,34,
		56,149,12,214,78,225,216,205,63,181,144,250,190,143,178,67,224,250,180,31,9,0,48,225,207,159,66,252,221,83,7,64,
		227,8,14,195,192,138,135,48,87,167,31,169,104,15,0,240,95,3,164,0,128,135,103,232,88,210,241,11,152,255,127,145,
		35,116,161,132,60,3,106,209,162,229,18,229,25,37,49,130,206,212,91,50,74,10,186,249,181,104,121,65,0,0,18,149,
		212,191,75,160,132,160,249,222,181,104,121,157,162,1,65,148,148,212,27,80,73,73,73,1,64,73,73,73,1,32,214,31,
		63,38,191,252,22,241,0,172,181,80,215,117,84,253,161,254,216,177,254,244,152,215,221,247,91,55,198,140,190,253,83,132,
		254,255,52,94,3,237,63,82,150,101,224,156,131,97,24,38,191,39,207,243,201,122,176,93,121,158,207,182,125,169,141,115,
		253,74,61,254,75,223,197,161,97,24,88,177,47,82,214,77,219,192,137,165,33,110,43,4,190,131,134,104,255,5,134,8,
		193,111,176,156,252,126,177,190,4,169,29,114,66,249,231,12,166,230,12,172,192,51,58,65,123,7,223,34,19,237,24,66,
		114,37,226,248,97,93,216,166,144,103,226,169,118,134,26,129,173,141,191,212,169,71,98,143,80,215,117,146,124,18,82,91,
		8,223,5,31,246,122,5,128,21,127,0,96,154,34,6,248,35,39,141,7,0,19,57,234,185,230,164,220,77,155,18,0,
		230,252,14,214,156,90,112,236,113,147,81,227,17,186,33,113,131,174,205,133,181,118,180,126,196,159,67,199,192,255,60,151,
		31,38,204,192,67,15,144,216,249,131,196,217,128,98,252,58,250,190,119,125,223,111,157,87,49,29,0,172,249,35,135,76,
		226,146,47,64,232,64,160,21,84,12,130,31,5,0,84,138,242,243,243,45,141,159,31,51,1,37,2,4,6,60,241,67,
		45,226,230,0,132,115,122,161,217,120,204,41,134,224,113,68,106,49,41,104,197,182,1,231,178,44,203,113,30,184,49,13,
		146,0,0,87,4,19,186,35,6,139,128,161,38,161,83,87,0,73,122,237,163,0,192,247,96,11,221,68,180,14,154,229,
		153,74,63,18,32,146,156,190,52,250,141,244,36,69,16,219,51,53,157,191,142,98,197,111,73,27,112,236,48,58,87,204,
		24,38,3,128,144,251,215,86,0,32,201,79,136,241,11,99,64,224,40,0,240,55,237,84,202,118,88,201,170,76,23,13,
		141,140,195,217,208,49,0,64,175,109,146,211,155,163,119,216,10,0,30,143,71,116,86,227,11,228,39,220,46,36,85,219,
		182,79,98,76,136,72,69,235,54,198,140,139,95,114,130,227,119,112,157,41,206,0,0,52,205,121,168,2,20,63,139,243,
		128,99,134,247,126,78,108,195,152,43,64,236,58,66,240,144,0,79,234,205,23,123,13,184,21,0,248,190,248,84,147,41,
		149,34,104,116,225,80,41,0,65,99,74,35,190,245,228,225,169,73,251,202,217,196,83,90,224,208,190,251,60,216,127,63,
		66,115,200,102,142,81,2,166,56,72,164,247,254,212,155,47,54,93,247,173,0,0,22,124,241,67,253,233,167,234,198,211,
		48,116,17,162,226,43,213,51,18,135,103,174,110,137,34,135,186,116,114,197,238,165,208,96,107,27,43,230,25,48,118,252,
		150,226,81,28,17,146,172,44,75,177,244,121,75,0,0,207,31,185,239,251,209,63,61,38,36,23,126,95,232,41,90,85,
		21,199,39,58,233,228,149,101,249,164,127,224,250,180,131,48,18,17,5,13,95,124,166,237,145,60,187,249,47,19,91,95,
		161,206,2,0,89,150,69,61,195,93,18,0,182,40,26,67,64,203,222,229,12,155,239,236,0,160,222,128,74,47,75,190,
		9,238,219,219,219,45,234,230,208,39,93,38,74,74,175,11,122,62,249,64,164,18,128,146,210,141,73,221,129,149,148,20,
		0,148,148,148,20,0,8,53,77,179,234,255,189,229,61,38,69,94,249,171,183,233,241,120,140,62,253,115,113,0,206,220,
		215,216,239,145,242,159,165,253,87,160,69,29,64,211,52,96,140,129,223,127,255,125,247,205,6,112,46,205,233,17,109,58,
		106,28,206,56,254,71,180,255,234,227,16,220,207,212,197,24,243,228,13,70,243,149,3,227,237,20,147,146,132,102,181,1,
		207,238,0,141,98,36,70,64,101,89,142,117,135,164,117,158,170,31,141,111,134,97,16,167,168,150,26,193,96,251,49,241,
		171,52,158,2,246,161,235,58,182,105,238,81,41,210,124,190,190,239,69,166,197,18,7,46,244,90,165,99,143,38,242,52,
		9,111,200,247,20,69,49,142,63,119,15,81,222,149,245,151,30,0,166,162,2,113,172,169,230,40,212,23,192,183,125,231,
		184,18,195,132,77,61,119,19,78,213,47,181,162,147,0,128,95,63,39,181,20,120,25,118,253,20,237,87,3,0,137,5,
		99,44,0,248,145,176,164,9,114,165,81,181,230,214,223,12,8,108,107,105,36,113,196,241,221,119,169,147,17,7,128,242,
		60,15,78,73,54,197,239,215,31,27,208,131,107,9,41,221,0,126,251,169,125,61,167,94,148,156,232,130,186,18,0,236,
		29,79,128,58,91,161,31,63,254,206,217,7,62,224,114,199,223,119,72,195,249,159,217,63,219,108,252,60,207,93,93,215,
		193,169,141,167,6,0,157,135,232,96,238,185,240,98,235,159,114,136,218,115,3,164,28,191,43,2,128,68,242,74,1,0,
		161,191,135,206,31,183,45,156,249,219,228,25,240,241,120,140,90,235,47,95,190,136,191,231,231,207,159,135,42,71,82,215,
		31,19,101,86,137,79,239,239,239,0,0,240,245,235,215,75,182,63,213,250,91,91,119,201,79,127,188,59,26,99,68,226,
		163,127,111,151,94,1,138,162,24,69,47,206,233,59,39,66,73,175,0,123,159,128,169,174,0,87,151,0,128,153,21,248,
		108,18,128,180,45,135,95,1,252,80,92,161,33,173,214,68,232,80,77,250,148,18,132,243,18,48,167,68,137,85,2,238,
		165,68,243,99,18,132,186,2,191,34,0,248,193,82,239,0,0,76,37,244,54,138,63,255,249,136,179,9,169,242,132,62,
		197,237,249,12,232,63,65,114,23,3,141,9,176,119,60,130,84,207,128,175,0,0,49,82,192,85,1,224,240,103,64,45,
		231,240,131,143,205,139,112,100,225,42,46,181,156,60,32,136,150,253,10,53,194,138,9,137,118,84,161,58,139,141,147,98,
		104,209,1,120,205,130,111,224,49,87,144,163,10,42,177,66,195,200,105,209,136,64,74,74,74,2,82,119,96,37,165,27,
		147,134,4,83,82,122,49,90,114,97,246,61,27,85,2,88,160,60,207,161,235,58,29,136,139,146,49,6,218,182,141,138,
		43,16,179,9,157,115,208,117,29,24,99,14,233,255,219,219,219,88,22,219,170,229,178,49,221,181,48,188,82,37,243,31,
		187,118,82,164,56,75,177,110,23,198,96,27,67,156,88,127,122,250,150,141,6,45,161,188,198,152,167,201,143,241,101,151,
		44,160,24,67,14,124,179,247,61,7,81,171,63,103,208,129,70,31,216,79,52,94,66,115,106,124,87,95,27,71,52,26,
		162,150,115,248,172,24,50,134,216,14,223,17,41,214,29,60,102,35,132,100,164,74,13,0,112,160,45,67,50,0,72,105,
		138,27,235,91,205,113,165,245,77,145,37,40,124,20,0,208,246,195,76,182,96,88,200,203,136,129,83,16,48,112,220,16,
		16,66,210,92,225,119,21,69,49,130,55,55,53,59,30,24,126,170,243,35,0,32,102,35,237,45,69,98,48,17,124,186,
		69,208,229,164,39,75,6,0,129,206,4,171,206,8,92,127,250,169,54,112,80,60,22,249,99,23,80,44,0,248,155,57,
		100,243,250,39,55,53,67,166,128,16,42,9,13,195,48,22,14,120,226,134,71,224,137,241,201,199,131,68,106,195,112,69,
		0,72,145,22,45,25,0,196,250,147,75,249,99,55,49,5,28,204,77,120,37,0,192,241,194,141,135,128,186,54,22,84,
		252,166,233,192,37,98,56,149,226,56,198,56,88,23,2,15,215,17,12,60,175,82,233,253,89,26,132,37,53,0,32,0,
		134,142,33,189,178,81,105,128,19,18,239,52,0,16,115,23,138,69,239,170,170,92,215,117,79,78,73,87,1,0,122,239,
		198,19,53,100,35,208,100,162,184,232,232,239,156,147,148,58,64,113,227,49,34,96,113,218,62,213,255,152,20,225,8,120,
		49,82,96,10,0,192,228,162,123,234,1,146,1,0,162,167,212,31,31,23,28,53,75,61,66,3,159,114,35,115,248,80,
		103,194,245,199,135,137,184,132,33,34,52,61,181,241,4,166,250,16,78,96,82,60,121,176,72,36,48,172,155,155,94,27,
		221,153,165,161,188,174,44,1,188,132,18,48,54,168,102,44,0,76,5,37,149,72,0,52,40,38,103,33,77,213,47,233,
		15,77,111,206,229,241,239,224,156,239,160,138,60,148,4,98,226,41,236,29,17,249,202,58,128,83,1,0,245,137,231,162,
		49,189,191,196,60,3,74,197,46,42,117,72,158,1,113,33,227,134,226,0,8,125,134,28,134,65,212,127,95,164,231,158,
		56,120,151,196,13,28,10,96,120,114,82,105,79,18,225,40,246,253,93,1,32,190,206,192,113,220,167,209,106,84,115,79,
		35,156,24,17,60,213,154,187,155,29,0,167,168,41,176,210,166,244,237,219,183,195,234,254,241,227,7,0,192,152,94,109,
		111,123,124,107,237,83,59,78,235,59,160,18,128,150,212,115,125,134,24,4,198,24,145,2,58,165,41,112,219,182,226,151,
		140,61,138,198,3,80,58,45,117,93,7,127,255,253,55,252,241,199,31,187,231,167,188,83,255,245,212,210,114,202,130,10,
		73,238,43,130,246,95,37,0,37,37,165,0,154,85,2,250,190,212,92,191,120,228,59,58,199,250,227,241,16,247,225,112,
		229,76,228,216,197,242,103,89,6,206,185,32,127,246,178,44,193,57,7,101,89,142,127,107,219,22,138,162,16,213,221,52,
		205,168,188,179,214,66,93,215,155,142,117,81,20,48,12,3,56,231,96,24,134,167,117,51,85,182,90,255,75,117,198,124,
		15,235,10,128,46,184,49,17,101,207,162,244,75,145,35,238,232,182,31,197,223,117,221,104,82,189,246,89,107,173,203,178,
		108,52,56,66,19,88,73,189,212,114,146,18,215,162,144,163,44,228,210,86,99,191,68,18,143,202,128,207,206,27,147,112,
		109,192,207,6,0,104,145,182,119,64,134,35,1,32,118,225,198,180,149,254,91,85,85,180,19,214,94,121,13,16,112,168,
		205,66,204,235,65,202,245,63,101,152,181,41,0,164,138,39,127,52,0,236,185,248,247,104,123,108,102,226,173,199,192,151,
		0,98,28,113,80,250,140,57,128,56,197,15,98,2,9,140,120,82,141,55,90,211,74,3,218,136,36,128,20,141,87,0,
		56,6,0,142,154,3,52,57,46,203,210,21,69,17,101,1,56,117,5,144,138,255,152,34,75,50,78,71,92,1,166,36,
		113,137,83,212,237,37,128,51,233,33,174,170,3,136,89,184,69,81,60,129,130,15,18,107,223,81,215,245,135,200,78,18,
		16,8,49,72,58,35,0,72,156,176,146,73,0,83,34,209,213,54,31,42,118,174,122,255,191,42,0,80,229,159,127,45,
		240,127,15,157,71,73,114,86,206,9,234,199,83,244,21,131,123,143,125,138,181,27,90,255,228,51,224,95,127,253,5,0,
		0,223,191,127,135,44,203,198,103,158,43,17,62,63,245,125,175,143,189,59,82,89,150,227,250,201,178,12,126,254,252,57,
		174,33,255,247,57,194,231,191,162,40,224,253,253,29,190,127,255,190,105,155,241,251,191,126,253,58,254,141,254,188,55,117,
		93,7,63,127,254,132,47,95,190,236,243,220,12,129,62,237,123,159,62,49,223,145,234,254,148,58,165,115,108,154,238,179,
		42,1,233,105,138,146,163,84,2,192,128,32,41,174,162,33,87,0,170,240,59,250,25,112,238,9,116,67,119,250,176,198,
		112,149,58,71,251,115,199,92,99,20,0,228,207,174,116,157,196,232,0,48,158,68,172,99,81,136,18,16,63,55,23,195,
		33,6,0,82,6,5,141,249,30,17,0,28,93,140,49,98,99,146,152,119,99,90,111,76,27,238,170,4,212,114,169,114,
		222,198,117,93,199,182,222,67,195,137,152,96,20,180,94,73,27,20,0,180,40,0,28,8,26,120,250,159,217,15,91,139,
		150,51,20,245,6,84,82,186,49,105,72,48,37,165,23,37,234,5,56,231,17,168,0,160,116,90,234,186,14,170,170,186,
		101,138,246,20,125,167,105,193,151,82,132,111,246,140,151,231,185,40,30,127,172,18,139,214,43,105,131,228,25,240,213,10,
		62,1,163,249,237,210,24,204,61,87,77,165,35,227,164,10,187,114,68,160,216,253,35,237,251,146,21,33,62,115,122,186,
		177,237,52,200,195,48,68,41,226,164,109,160,245,74,218,160,0,0,163,13,5,102,249,9,177,195,159,178,121,192,55,117,
		248,39,97,204,222,105,178,142,6,128,35,234,246,147,203,2,44,38,152,61,167,37,31,68,188,193,199,110,224,179,216,1,
		92,113,209,251,191,151,101,57,142,223,48,12,227,66,92,27,127,106,137,42,77,236,34,41,104,56,132,167,168,49,102,76,
		144,195,113,72,74,97,0,23,35,65,248,41,221,22,82,188,157,23,0,164,111,240,177,0,144,194,14,32,38,171,210,43,
		1,0,254,139,207,179,107,215,9,0,248,224,5,184,151,83,151,159,210,12,159,147,253,232,88,33,129,78,142,6,0,180,
		100,108,154,230,41,213,219,174,0,112,231,66,23,205,86,145,108,174,2,0,126,138,179,181,117,21,155,209,135,2,9,231,
		14,141,82,7,110,112,234,147,96,140,25,239,215,33,18,97,138,253,131,128,36,53,131,70,208,93,73,109,167,0,112,38,
		0,144,216,130,75,105,47,0,192,141,131,98,252,90,221,120,135,197,147,95,26,90,172,105,26,209,216,135,140,237,30,0,
		96,173,141,146,124,168,75,243,194,21,74,1,96,139,34,189,2,92,25,0,82,22,12,70,138,215,1,201,107,18,138,193,
		161,38,225,177,206,91,169,175,191,41,172,89,197,206,64,10,0,90,174,174,81,231,2,0,94,1,80,123,78,175,0,101,
		89,142,79,115,33,82,69,204,254,193,122,83,232,142,20,0,180,92,174,76,197,162,144,72,0,220,43,192,156,18,208,167,
		43,40,1,147,1,128,2,129,150,189,75,150,101,79,97,185,165,207,128,92,37,224,210,51,32,254,141,251,12,152,42,166,
		195,86,0,160,206,64,74,74,55,38,245,5,80,82,82,0,80,82,82,82,0,80,82,82,186,21,125,210,33,80,82,122,
		29,90,203,4,236,187,5,171,18,80,73,73,175,0,231,37,99,140,40,185,71,219,182,99,110,244,166,105,118,107,111,215,
		117,224,156,131,60,207,199,191,229,121,14,206,57,118,112,7,78,62,120,127,204,104,174,123,99,76,244,105,194,105,75,81,
		20,227,56,96,191,49,81,75,40,61,30,143,49,65,72,93,215,183,218,148,41,198,143,53,207,103,46,77,211,176,179,196,
		82,39,8,73,82,137,169,207,134,242,99,146,9,106,184,130,237,225,58,183,164,200,137,16,234,188,146,114,190,250,190,127,
		122,127,199,152,251,156,57,192,56,14,216,7,184,137,13,68,170,241,99,148,115,154,112,82,151,80,174,49,199,148,7,87,
		168,37,217,82,102,150,208,9,64,103,150,60,207,71,243,81,73,98,203,88,75,178,189,163,34,227,66,5,18,71,193,90,
		59,206,159,191,176,97,37,57,230,48,12,79,110,196,176,163,217,241,17,190,19,116,252,40,161,59,58,103,252,46,11,0,
		83,230,152,92,16,136,77,110,234,111,118,4,5,206,119,88,107,93,215,117,163,83,135,180,29,18,62,180,162,219,59,22,
		65,215,117,227,60,181,109,59,218,207,163,4,82,20,69,48,16,227,252,33,128,223,1,0,232,248,81,79,74,92,63,156,
		241,59,5,0,64,132,29,120,85,85,238,241,120,140,39,64,168,24,155,98,242,230,82,69,199,46,160,20,237,8,5,80,
		106,79,191,145,232,184,216,222,37,207,69,96,152,228,98,1,129,57,47,48,211,131,165,150,100,83,140,159,31,145,106,131,
		118,108,235,210,201,245,229,166,131,141,121,230,83,32,248,94,0,144,114,225,72,227,33,34,31,205,111,39,185,130,28,9,
		0,49,197,90,251,212,95,255,247,189,230,81,18,67,192,231,165,87,168,203,1,64,219,182,226,164,162,56,113,71,160,118,
		204,21,224,104,31,124,255,254,79,83,165,237,41,194,226,207,210,43,64,204,154,163,247,102,255,247,51,3,192,212,21,160,
		174,235,81,47,112,185,43,0,222,129,37,87,0,140,101,134,34,45,71,132,227,68,178,221,66,9,184,183,232,8,51,169,
		209,170,170,218,245,37,32,165,18,48,22,0,168,46,226,42,161,197,125,37,32,144,168,62,151,84,2,74,0,96,78,9,
		24,250,20,72,221,72,165,27,56,230,25,240,232,43,192,212,24,160,91,235,139,62,99,109,126,5,208,103,192,131,34,185,
		224,201,197,177,3,40,138,98,156,236,44,203,198,160,16,214,90,182,61,193,81,253,79,113,223,163,161,172,183,22,187,167,
		230,192,15,72,185,231,9,156,90,9,120,132,36,176,227,248,157,111,3,24,99,196,169,189,95,169,164,184,62,104,64,23,
		45,75,229,148,166,192,101,89,194,223,127,255,173,134,218,2,66,19,96,99,204,104,2,252,254,254,174,3,163,180,191,41,
		112,76,106,175,216,152,240,119,149,0,80,122,58,34,171,142,150,235,21,245,6,84,82,186,49,105,64,16,37,37,5,128,
		141,238,22,255,184,51,42,41,41,93,12,0,208,31,219,90,11,143,199,99,247,134,81,127,240,170,170,196,252,49,237,71,
		63,254,61,227,9,0,192,147,47,56,45,87,163,60,207,193,90,43,230,47,138,98,156,195,45,253,225,215,218,90,215,245,
		234,26,92,226,47,138,2,218,182,21,175,215,44,203,96,24,134,237,14,106,152,49,166,64,119,218,24,111,54,233,19,20,
		77,139,36,249,14,52,126,137,241,39,239,186,110,55,227,21,152,49,226,1,128,203,62,227,197,186,242,98,76,0,106,78,
		188,119,91,243,60,15,202,207,183,212,215,144,182,47,173,215,182,109,163,76,153,65,98,7,32,5,128,212,182,240,93,215,
		69,185,180,114,82,67,193,70,47,31,156,190,79,141,215,149,223,241,83,25,66,237,237,141,71,55,102,232,139,212,20,63,
		119,243,250,235,53,207,115,177,33,87,224,186,59,62,185,37,204,60,103,245,125,47,70,190,163,34,202,40,0,164,3,128,
		61,37,128,169,182,214,117,205,178,30,245,249,203,178,12,54,65,158,91,175,28,0,74,10,0,112,160,29,0,16,183,214,
		24,115,226,35,76,128,99,1,192,119,70,138,217,68,103,176,2,140,169,191,40,138,113,14,185,1,97,232,231,67,83,132,
		209,182,74,78,94,202,111,140,97,241,79,173,215,199,227,177,135,15,195,121,109,225,175,122,226,197,166,152,70,255,133,87,
		0,128,35,74,93,215,79,215,86,255,247,80,29,18,61,121,209,173,154,171,131,2,207,47,131,19,31,131,74,61,220,250,
		47,47,1,220,25,0,252,19,69,109,249,101,81,145,240,46,237,255,30,2,32,190,212,136,110,234,33,252,126,32,28,137,
		66,187,170,170,167,54,112,234,7,226,18,31,112,117,210,5,179,5,0,224,2,160,57,230,21,0,246,41,120,90,226,134,
		247,127,95,227,245,69,119,26,151,66,194,207,85,104,103,89,246,180,113,57,245,211,205,111,173,13,209,161,237,23,84,241,
		14,11,57,101,223,21,0,246,191,2,76,41,221,56,243,56,197,111,140,113,77,211,4,43,243,252,151,3,78,253,120,224,
		160,82,113,119,29,128,2,192,191,18,0,253,87,1,96,191,34,85,2,214,117,189,248,185,181,121,104,154,102,242,132,231,
		40,180,151,36,136,144,117,128,186,6,198,203,129,46,152,179,234,61,142,6,0,188,119,94,37,154,142,150,127,1,128,234,
		15,202,178,92,210,7,232,160,41,0,44,219,99,168,4,114,157,66,163,105,249,52,163,127,208,65,83,0,152,239,199,48,
		12,187,38,23,209,146,46,184,40,141,9,185,112,181,89,54,72,137,177,65,70,237,171,196,158,30,45,162,36,193,44,176,
		237,40,186,30,177,129,164,150,148,103,42,87,94,3,87,239,251,142,253,95,30,132,152,128,146,49,206,52,93,215,185,170,
		170,68,245,163,18,36,38,60,120,172,29,122,10,103,170,179,128,192,21,215,64,138,57,60,178,239,41,246,64,72,223,53,
		34,208,70,100,173,133,207,159,63,3,0,64,223,247,240,235,175,191,234,160,40,157,146,22,239,128,177,174,192,119,231,95,
		80,190,44,138,141,177,207,169,212,156,152,155,80,34,246,25,247,234,252,169,244,57,23,225,95,79,206,193,141,73,174,252,
		114,126,244,43,95,3,130,144,231,59,105,134,101,5,128,155,3,0,42,64,112,49,114,239,49,202,47,227,199,77,143,119,
		62,201,2,64,195,35,106,145,134,65,70,184,25,150,95,124,225,235,56,204,1,0,181,38,146,136,193,202,47,227,167,167,
		191,116,1,224,233,79,53,199,89,150,185,174,235,130,53,218,10,0,55,6,0,223,115,138,155,93,85,249,101,252,254,233,
		47,21,101,83,44,126,189,2,220,7,0,62,4,5,197,224,139,223,190,125,123,250,247,203,151,47,65,26,69,229,151,241,
		127,253,250,21,0,0,254,243,159,255,168,90,90,233,184,87,0,223,113,129,90,132,133,58,98,40,63,143,63,70,225,55,
		87,191,94,1,244,10,192,190,2,160,221,247,28,173,89,36,41,191,140,159,102,130,141,5,0,52,126,82,37,160,2,0,
		251,10,80,150,37,0,0,252,249,231,159,240,246,246,54,150,63,255,252,243,233,255,231,72,249,249,252,121,158,67,81,20,
		240,253,251,247,39,158,183,183,183,127,69,52,70,142,128,255,254,247,191,240,243,231,207,49,78,189,115,110,140,51,143,237,
		8,22,13,35,115,19,92,157,255,118,87,128,41,241,145,158,108,107,39,136,242,243,249,125,205,127,42,101,22,13,46,170,
		134,64,170,4,100,61,3,106,185,126,81,95,126,45,33,69,125,1,94,152,140,49,48,12,195,211,117,66,73,137,210,39,
		29,130,23,188,211,253,115,215,125,127,127,15,126,190,84,82,29,192,38,254,212,82,159,252,88,127,236,35,125,177,207,16,
		143,224,232,114,35,127,250,77,251,176,67,217,46,162,76,172,79,126,140,63,246,209,190,216,71,199,35,56,11,8,188,186,
		63,253,214,125,80,29,128,210,105,40,207,115,248,241,227,135,14,196,11,209,47,83,247,71,191,180,109,11,198,152,224,251,
		39,125,123,246,255,30,66,77,211,140,111,216,214,90,104,154,134,213,169,187,243,199,82,211,52,48,12,195,211,252,87,85,
		5,125,223,127,152,215,185,249,15,253,251,171,241,27,99,192,57,7,125,223,127,248,191,190,239,193,57,183,186,151,98,109,
		22,140,49,99,93,195,48,172,207,25,4,188,161,90,107,131,98,141,207,153,190,134,190,105,206,249,178,135,228,104,87,254,
		116,207,135,83,20,146,89,103,110,158,67,231,255,234,252,64,172,49,167,66,115,135,92,7,99,223,255,209,242,51,48,168,
		205,114,229,89,150,141,139,34,228,46,67,137,230,71,227,100,53,161,137,21,49,77,120,72,98,197,187,243,227,124,249,243,
		20,122,7,158,138,37,64,131,155,40,0,132,111,74,12,8,59,247,123,232,30,66,226,232,49,144,2,221,209,215,59,26,
		106,9,231,75,0,84,106,8,25,64,180,164,243,53,199,168,73,13,181,196,187,43,63,206,149,181,118,220,196,125,223,63,
		37,167,228,212,143,223,133,127,15,89,192,10,0,207,241,249,155,166,25,95,68,66,67,195,197,72,96,148,31,165,142,149,
		131,99,185,98,233,0,250,126,241,33,252,75,159,81,254,240,5,136,27,23,55,109,232,51,152,95,7,2,201,157,54,112,
		42,0,0,207,201,75,114,130,25,44,54,213,0,0,4,232,73,68,65,84,195,132,147,25,135,159,166,152,143,146,0,164,
		3,56,21,223,78,55,240,53,0,0,37,135,187,109,224,148,0,64,55,46,199,22,33,21,0,160,228,177,162,63,90,239,
		40,222,37,56,87,0,202,71,21,75,33,34,168,239,184,130,210,68,168,8,125,87,126,186,249,49,33,9,254,28,178,0,
		17,52,240,250,134,60,156,43,136,2,64,60,223,28,133,218,19,208,58,125,163,52,17,0,160,86,145,163,4,244,53,162,
		92,37,32,222,67,243,60,23,41,209,238,200,143,0,64,83,67,251,191,135,138,172,168,193,166,74,224,144,250,167,20,198,
		184,8,67,238,176,87,231,223,10,0,56,198,68,204,235,251,54,207,128,83,39,139,62,3,158,251,25,208,127,62,146,212,
		79,211,161,75,194,146,95,157,63,37,0,196,88,225,162,210,49,32,34,213,58,0,180,109,43,86,36,209,55,208,208,78,
		209,164,22,214,90,81,88,238,59,243,75,10,46,150,186,174,159,36,129,97,24,216,166,204,101,89,62,133,70,27,134,33,
		72,122,120,21,254,179,0,0,189,14,206,249,100,168,41,176,146,154,250,222,152,20,0,148,148,110,76,191,232,16,40,41,
		41,0,40,37,166,199,227,1,214,90,176,214,174,58,99,40,41,233,21,224,197,72,211,131,43,93,90,2,104,154,6,156,
		115,144,231,185,248,203,243,60,7,231,156,216,157,53,203,178,32,247,201,165,246,215,117,13,0,160,33,161,5,20,187,6,
		142,156,255,163,251,126,165,254,191,108,68,151,152,168,60,177,17,97,168,21,94,168,19,200,25,203,85,163,50,165,152,195,
		59,68,52,210,43,128,146,146,94,1,158,201,24,3,93,215,141,145,73,250,190,103,137,33,202,47,231,71,209,211,57,7,
		93,215,65,158,231,99,116,158,97,24,160,170,42,150,18,82,42,130,62,30,143,167,122,49,233,233,93,248,241,218,24,115,
		117,188,10,127,144,57,106,104,94,57,229,151,243,251,126,19,115,180,102,149,70,131,120,132,242,172,241,75,82,164,95,149,
		31,52,51,16,136,93,17,149,95,206,111,173,29,237,205,41,31,254,45,212,43,15,1,40,207,115,86,48,151,41,126,90,
		111,168,51,204,213,249,21,0,254,25,180,186,174,131,93,121,149,63,13,127,81,20,31,248,184,57,238,82,45,156,41,135,
		176,59,240,223,9,0,126,153,187,63,97,84,83,73,102,25,229,151,241,151,101,9,93,215,129,181,246,233,206,234,103,13,
		62,42,213,87,150,101,183,230,127,85,154,13,12,97,140,121,186,79,133,34,143,242,203,248,231,248,240,41,9,61,246,214,
		92,115,81,122,40,138,98,188,2,112,78,63,95,132,230,150,171,243,223,254,10,64,93,33,25,177,197,148,63,146,127,142,
		143,171,204,154,82,130,113,222,163,231,148,104,161,239,209,87,231,191,61,0,96,20,25,28,52,60,69,66,23,145,242,203,
		248,243,60,31,249,218,182,117,89,150,61,69,4,26,134,33,216,168,8,13,145,164,121,233,168,79,188,36,38,193,213,249,
		245,21,64,203,229,11,2,135,36,55,161,150,123,20,181,4,124,113,50,198,192,48,12,135,41,14,149,206,77,159,116,8,
		94,84,179,251,143,5,216,251,251,187,232,37,67,233,30,164,18,128,146,210,141,73,3,130,40,41,41,0,164,167,20,254,
		248,87,246,71,215,120,4,247,142,39,144,162,253,187,93,23,97,65,139,108,173,117,85,85,61,165,58,134,192,39,136,24,
		127,124,184,184,63,122,138,254,199,250,179,159,161,220,57,158,128,180,253,177,207,127,204,182,47,191,165,74,108,168,181,156,
		163,164,88,72,64,108,20,116,76,175,55,111,107,229,151,37,241,37,203,50,248,237,183,223,192,90,11,93,215,177,52,208,
		206,57,40,138,98,244,201,230,250,212,83,94,137,63,247,221,249,99,137,214,223,182,45,244,125,31,28,220,116,202,15,157,
		227,219,126,101,126,28,51,244,59,192,107,200,48,12,236,57,144,4,150,149,212,63,155,34,9,45,201,208,160,36,54,191,
		121,104,122,169,57,83,206,208,212,76,119,231,143,45,83,245,115,164,192,169,19,76,146,221,246,138,252,152,71,16,247,10,
		250,111,72,114,251,73,230,95,80,127,120,126,180,80,127,106,223,246,154,235,80,227,39,104,12,76,115,172,252,11,139,135,
		51,254,152,26,172,44,203,167,252,114,161,65,69,238,12,0,56,94,184,87,48,223,34,39,46,36,18,234,141,56,243,47,
		168,127,222,41,5,153,184,185,253,144,104,50,209,187,44,128,163,249,83,0,0,253,44,218,209,75,252,241,239,56,254,89,
		150,61,141,21,215,145,44,118,255,8,234,63,255,4,96,167,148,223,237,170,132,194,36,165,10,192,32,146,96,67,221,183,
		15,174,127,93,2,224,118,36,85,7,244,10,32,227,143,213,38,211,104,68,143,199,99,148,0,185,243,143,119,86,108,63,
		119,3,94,149,31,159,126,113,31,73,66,210,211,43,23,119,254,153,245,207,63,255,73,149,80,177,0,112,247,160,148,71,
		7,181,156,211,1,133,206,255,82,56,179,59,240,251,243,199,85,222,198,40,1,5,245,175,251,83,115,252,208,83,0,0,
		118,130,214,207,29,196,187,243,199,20,170,248,147,204,191,49,230,67,12,3,206,252,95,157,223,223,196,210,43,88,76,114,
		25,70,253,106,120,161,69,13,127,82,23,4,16,110,36,226,84,134,64,161,245,171,51,144,210,36,253,248,241,67,7,33,
		1,125,251,246,237,244,245,43,98,107,209,146,248,21,69,26,134,44,133,4,192,169,255,127,120,142,103,38,93,50,134,131,
		0,0,0,0,73,69,78,68,174,66,96,130
	};

	struct Glyph {
		const int x;
		const int y;
		const int width;
		const int height;
		const K::ImageChannel& font;
		Glyph(const int x, const int y, const int width, const int height, const K::ImageChannel& font) : x(x), y(y), width(width), height(height), font(font) {;}
		int getWidth() const {return width;}
		int getHeight() const {return height;}
		float get(const int _x, const int _y) const {return font.get(x+_x, y+_y);}
	};

	class Font {

	private:

		K::ImageChannel img;
		int cols;
		int rows;

	public:

		static Font getDefault() {
			Font fnt;
#ifdef WITH_PNG
			fnt.img = K::ImageFactory::readPNG(FONT1_PNG);
#endif
			fnt.cols = 16;
			fnt.rows = 16;
			return fnt;
		}

		/** swap black/white */
		Font inverted() const {
			Font copy = *this;
			copy.img.forEachModify([] (const int x, const int y, const float val) {
				(void) x; (void) y; return 1.0f - val;
			});
			return copy;
		}

		Glyph getGlyph(const char c) const {
			const int gw = img.getWidth() / cols;
			const int gh = img.getHeight() / rows;
			const int col = c % cols;
			const int row = c / cols;
			const int x = col * gw;
			const int y = row * gh;
			return Glyph(x,y, gw,gh, img);
		}

	};

}

#endif // FONTS_H
