//#define null NULL



// types
typedef unsigned char		u8;
typedef signed char			s8;
typedef unsigned short		u16;
typedef signed short		s16;
typedef unsigned int		u32;
typedef signed int			s32;
#if defined (CC_TARGET_OS_IPHONE)
typedef unsigned long long	u64;
typedef long long			s64;
#else
typedef unsigned long long		u64;
typedef long long				s64;
#endif

//typedef unsigned long			u64;
//typedef long 					s64;

//typedef unsigned __int64		u64;
//typedef __int64					s64;

//typedef unsigned long long		u64;
//typedef signed long	long		s64;

#define byte u8

#if defined WIN32 || defined ANDROID
typedef u32 UInt32;
#endif

#define U_STR	wstring	
typedef u32 Pixel_Type;

#define INT_TYPE                                                                    int
#define UNBYTE_TYPE																	unsigned char
#define BYTE_TYPE                                                                   signed char
#define SHORT_TYPE                                                                  short
#define BOOLEAN_TYPE                                                                bool
#define FLOAT_TYPE																	float
#define LONG_TYPE                                                                   s64

#define GRAPHICS_REF                                                                CGraphics*
#define IMAGE_REF                                                                   CImage*
#define ASPRITE_REF																	ASprite*
#define SPRITE_REF																	ASprite*

#define STRING_REF                                                                  char*
#define STRING_ARRAY_REF                                                            char**
#define STRING_ARRAY_ARRAY_REF														char***

#define INT_ARRAY_REF                                                               int*
#define INT_ARRAY_ARRAY_REF                                                         int**
#define INT_ARRAY_3D_REF															int***
#define INT_ARRAY_4D_REF															int****

#define UNBYTE_ARRAY_REF															unsigned char*
#define BYTE_ARRAY_REF                                                              signed char*
#define BYTE_ARRAY_ARRAY_REF                                                        signed char**
#define BYTE_ARRAY_3D_REF                                                           signed char***

#define UNSHORT_ARRAY_REF                                                           unsigned short*
#define SHORT_ARRAY_REF                                                             short*
#define SHORT_ARRAY_ARRAY_REF														short**
#define SHORT_ARRAY_3D_REF														    short***
#define SHORT_ARRAY_4D_REF														    short****
#define SHORT_ARRAY_5D_REF															short*****

#define LONG_ARRAY_REF                                                              s64*
#define LONG_ARRAY_ARRAY_REF                                                        s64**

#define FLOAT_ARRAY_REF																float*
#define FLOAT_ARRAY_ARRAY_REF														float**

#define OBJECT_REF                                                                  void*
#define OBJECT_ARRAY_REF                                                            void**

#define SPRITE_ARRAY_REF                                                            ASprite**

#define INT_TYPE                                                                    int
#define UNBYTE_TYPE																	unsigned char
#define BYTE_TYPE                                                                   signed char
#define SHORT_TYPE                                                                  short
#define BOOLEAN_TYPE                                                                bool
#define FLOAT_TYPE																	float
#define LONG_TYPE                                                                   s64

#define DEBUG_ASPRITE_LOADING 0

#define DEBUG_ASPRITE_LOADING_FMODULES		0

#define DEBUG_ASPRITE_LOADING_MODULES 0
#define DEBUG_ASPRITE_LOADING_IMAGE_DATA		0


#define k_param_common_layerId	0
#define k_param_common_posX		1
#define k_param_common_posY		2
#define k_nParams_common		5

#if _DEBUG_PRINT
#define TRACE printf
#else
#define TRACE
#endif


#define GAME_NEW new

#define NEW		new



#define NOTNULL(x)	(x)

#define INIT_ARRAY(type,objects,length)		{\
	objects = GAME_NEW type[length];\
}

#define INIT_ARRAY_ARRAY(type,objects,length1,length2)		{\
	DELETE_ARRAY_ARRAY(objects,length1);\
	INIT_ARRAY(type*,objects,length1);\
	for(INT_TYPE iaa=0;iaa<length1;iaa++)\
	INIT_ARRAY(type,objects[iaa],length2);\
}

#define INIT_ARRAY_REF(type,objects,length)		{\
	DELETE_ARRAY_REF(objects,length);\
	objects = GAME_NEW type[length];\
}\


#define UNLOAD( variable )	{\
	variable = NULL;\
}

//实例化的指针删除时用这个
//GameLibPlayer *abc = new GameLibPlayer(this);
#define DELETE_VAR( variable )															if NOTNULL(variable)\
{\
	delete variable;\
	variable = NULL;\
}

//非实例化的指针删除时用这个
//int *abc = new int [10];
#define DELETE_ARRAY( variable )													if NOTNULL(variable)\
{\
	delete[] variable;\
	variable = NULL;\
}

//二位可实例化指针用这个
//ASprite **abc;
//abc = new ASprite *[10];
//循环 -> abc[i] = new ASprite();
#define DELETE_ARRAY_REF(objects,length)		if NOTNULL(objects)\
{\
	for(INT_TYPE dar=0;dar<length;dar++)\
	DELETE_VAR(objects[dar]);\
	DELETE_ARRAY(objects);\
}\
												else objects = NULL;
//二位基础类型指针删除用这个
//int **abc;
//abc = new int *[10];
//循环 -> abc[i] = new int [10];
#define DELETE_ARRAY_ARRAY(objects,length)											if NOTNULL(objects)\
																					{\
																					for(INT_TYPE daa=0;daa<length;daa++)\
																					DELETE_ARRAY(objects[daa]);\
																					DELETE_ARRAY(objects);\
																					}\
																					else objects = NULL;
#define DELETE_ARRAY_2_REF(objects,length1,length2)	if NOTNULL(objects)\
														{\
														for(INT_TYPE daar=0;daar<length1;daar++)\
														DELETE_ARRAY_REF(objects[daar],length2);\
														DELETE_ARRAY(objects);\
														}\
														else objects = null;

#define DELETE_ARRAY_3_i(objects,length1,length2)	if NOTNULL(objects)\
												{\
												for(INT_TYPE da3i=0;da3i<length1;da3i++)\
												DELETE_ARRAY_ARRAY(objects[da3i],length2[da3i]);\
												DELETE_ARRAY(objects);\
												}\
												else objects = null;




#define LAYER_TAG	20
#define HERO_TAG	21
#define RALF_TAG	22
#define PLAYER_2_TAG	23
#define RALF1_TAG	24

#define SCENE_FIRST			0
#define SCENE_MM			1
#define SCENE_SINGE_GAME	2
#define SCENE_HOME			3
#define SCENE_PVP			4
#define SCENE_LOADING		5


#define QUADRANT_1		0
#define QUADRANT_2		1
#define QUADRANT_3		2
#define QUADRANT_4		3
#define QUADRANT_ORIGIN	4


#define ACTOR_HIT_TIME_SCALE	0.03f
#define GROUP_OVER_TIME_SCALE	0.1f