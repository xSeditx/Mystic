#ifndef _COMMON_H
#define _COMMON_H
#pragma once

// Standard Library includes
#include<iostream>
//#undef _DEBUG
// ===================================================================================================================================================
// COMPILER BEHAVIOR
// ===================================================================================================================================================
#define _MYDEBUG
#define _NO_WARNING

#define Lambda(Cap)  [#Cap]
#define FALSE 0
#define TRUE !FALSE

#ifdef DLL_CONFIG
#    ifdef MY_EXPORTS
#    define MYSTIC __declspec(dllexport)
#else
#    define MYSTIC __declspec(dllimport)
#endif
#    else
#    define MYSTIC // What goes here?
#endif

#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

#ifdef _NO_WARNING
#    pragma warning( disable : 4244 60 ) // Disable Typecast warnings
#    pragma warning( disable : 4267 60 ) // Disable size_t to GLsizei possible loss of data warning since they are essentially the same thing anyway
#    pragma warning( disable : 4018 )    // Disable Signed/ Unsigned Type Mismatch
#    pragma warning( disable : 4049 )    // Disable locally defined symbol X imported 
#    pragma warning( disable : 4522 )    // Disable multiple assignment operators specified
#endif

#define OPTIMIZATION_OFF()  __pragma(optimize("",off))\
__pragma(message("WARNING: Global Optimization Disabled"))\
 
#define OPTIMIZATION_ON()  __pragma(optimize("", on))\
__pragma(message("WARNING: Global Optimization Enabled"))\

// ===================================================================================================================================================
#define cpuid(func,a,b,c,d)\
	asm {\
	mov	eax, func\
	cpuid\
	mov	a, eax\
	mov	b, ebx\
	mov	c, ecx\
	mov	d, edx\
	}


#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
// ===================================================================================================================================================
// TYPES FOR OUR ENGINE
// ===================================================================================================================================================
/// Naming Convention. [sign][precision][type][modifier]
///  ex: iVec2 Integer of Vec type with 2 members
///  ex: Uint8 Unsigned Integer 8 bytes

// Float Vectors
typedef glm::vec2  Vec2;
typedef glm::vec3  Vec3;
typedef glm::vec4  Vec4;
// Integer Vectors
typedef glm::ivec2  iVec2;
typedef glm::ivec3  iVec3;
typedef glm::ivec4  iVec4;
// Matrices
typedef glm::mat3  Mat3;
typedef glm::mat4  Mat4;
// Colors
typedef glm::vec3 fRGB;
typedef glm::vec4 fRGBA;
// Packed Integers
typedef glm::uint8 Uint8;
typedef glm::uint16 Uint16;
typedef glm::uint32 Uint32;
typedef glm::uint64 Uint64;
// Object Tags
typedef unsigned int TagID;
typedef std::string  TagName;

// Engine Types to ensure proper usage
typedef unsigned int MysticKey;


using GPUptr = uint64_t;
// ===================================================================================================================================================


std::ostream& operator<<(std::ostream &lhv, Vec2 const &rhv);
std::ostream& operator<<(std::ostream &lhv, Vec3 const &rhv);
std::ostream& operator<<(std::ostream &lhv, Vec4 const &rhv);


// ===================================================================================================================================================
//_____________________________________________________________  DEFINES  ____________________________________________________________________________
/* The MACRO Creates Anon Mutex and LockGuard to protect Named Scope */
#define atomic_region(x) std::mutex CONCATENATE(x, ATOMIC_REGION); \
std::lock_guard<std::mutex> CONCATENATE(x,Guard)(CONCATENATE(x, ATOMIC_REGION));


#ifndef M_PI
#    define     M_E           2.71828182845904523536
#    define     M_LOG2E       1.44269504088896340736
#    define     M_LOG10E      0.434294481903251827651
#    define     M_LN2         0.693147180559945309417
#    define     M_LN10        2.30258509299404568402
#    define     M_PI          3.14159265358979323846
#    define     M_PI_2        1.57079632679489661923
#    define     M_PI_4        0.785398163397448309616
#    define     M_1_PI        0.318309886183790671538
#    define     M_2_PI        0.636619772367581343076
#    define     M_1_SQRTPI    0.564189583547756286948
#    define     M_2_SQRTPI    1.12837916709551257390
#    define     M_SQRT2       1.41421356237309504880
#    define     M_SQRT_2      0.707106781186547524401
#endif                      
#define RADIANS(x)            ((x) * 0.01745329251)
#define DEGREES(x)            ((x) * 57.2957795131)

#define pure_virtual  0 // a Macro to make the code appear more like human language
#define _static 

#define _WORKING     1 // Because why not
#define _NOT_WORKING 0
#define FALSE  0 // OLD SCHOOL C Boolean
#define TRUE   !FALSE


#undef  APIENTRY     // Prevent the APIENTRY redefinition in glfw
#define NOMINMAX     // Prevents a problem in AssImp in the Material.inl file  ErrorNO: 2589
#include<Windows.h>  // The Above must be defined before this is included

//====================================================================================================================================================
//_____________________________________________________________  MACROS  _____________________________________________________________________________

#define Print(x)                   std::cout << x << "\n" // Simple Console Logger


#define PrintXY(S,xx,yy)           SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {xx, yy});\
                                   Print(S);
#define MYSTIC   // This will be for possible DLL exportation later on in our applications life

#define BIT(x) (1 << x) // Sets Specific bit of a number o

#define GL_Color(c)                               (c * (.0039215686627451f)) // Normalizes RGB 255 to OpenGLs Color System
#define NormalizedColor(r,g,b)                    GL_Color(r), GL_Color(g), GL_Color(b)
#define for_loop(_ITR, _COUNT)                    for(unsigned int _ITR = 0; _ITR < (_COUNT); _ITR++) // Basic loop later possibly implementing Loop unrolling via this

#define CONCATENATE_IMPL(_name, _number)          _name##_number
#define CONCATENATE(_name, _number)               CONCATENATE_IMPL(_name, _number)
#define MAKE_NAME(_name, _number)                 #_name#_number

#ifdef __COUNTER__
#define ANON_VAR(str) CONCATENATE(str, __COUNTER__)
#else
#define ANON_VAR(str) CONCATENATE(str, __LINE__)
#endif 
// USEFUL FOR END OF CODING SESSION TO MARK THE STATE OF ONES PROGRESS 
// AND ANY NOTES THEY WISH TO LEAVE FOR THEMSELVES WHEN THEY PICK BACK UP

#define TODO(x) do{\
static bool _seenAlready = false;\
if(!_seenAlready){\
_seenAlready = true;\
__pragma(message(#x))\
Print("File: " << __FILE__); \
Print("Line: " << __LINE__); \
Print("Message: " << #x); \
}\
}while(0)



#define NO_COPY_OR_ASSIGNMENT(Class_X)	void operator=(const Class_X&) = delete;\
Class_X(const Class_X&) = delete
 
#define FINISHED_TODAY(x) TODO(x)\
__debugbreak();

/// TODO: Create a DEBUG BREAK system that breaks the program whenever said breakpoint VARIABLE is triggered
/// NEED A SINGLETON MANAGER FOR THAT, Pointers, a thread that cycles your variables then breaks at its current program position

#define RANDOM_RANGE( min, max )      ((rand() % (int)(((max) + 1) - (min))) + (min))  
#define RANDOM(x)                  ((rand() * (1.0f / (1.0f + RAND_MAX))) * (x))
#define RANDOM_SPREAD(x)            (RANDOM(x * 2.0f) - (x))                                    // Provides a Random number +/- the Given value

#define MIN(a,b)                   ((a<b) ? a : b)
#define MAX(a,b)                   ((a>b) ? a : b)

#define PICK_ONE(a, b)             ((RANDOM(1) > .5) ? a : b) // Randomly pick one of the two values. Useful for Genetic Algorithms.

#define WaitKey                    system("PAUSE"); // TODO: Perhaps a Getch or CIN could be appropriate as I intend to make this Library Linux friendly for use with my Raspberry PI

#define Squared(x)                  ((x) * (x))


#ifdef _DEBUG
#define ASSERT(x) if(!(x)){exit(EXIT_FAILURE);}
#define _GL(x)           GLClearError();\
                                    (x);\
       ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define _GL(x)  x
#endif

#ifdef _MY_DEBUG
#    define DEBUG_TRACE(x)         std::cout << x << __FUNCTION__ << std::endl
#else
#    define DEBUG_TRACE(x)
#endif
//____________________________________________________________________________________________________________________________________________________
// ===================================================================================================================================================


// ===================================================================================================================================================
// TEMPLATES
// ===================================================================================================================================================
 

// BIT MANIPULATION STUFF
// ===================================================================================================================
// USE: Value & Mask(X)

// BINARY CONVERTER
// https://www.mathsisfun.com/binary-decimal-hexadecimal-converter.html

/* Defines for accessing the upper and lower byte of an integer. */
#define  LOW_BYTE(x)    (x & 0x00FF)
#define  HI_BYTE(x)     ((x & 0xFF00) >> 8)

#define  BIT(x)         (1 << x) // GET THE STATE OF A SINGLE BIT AT X POSITION
#define  MAX_INTEGER    0xFFFFFFFF

#define  BIT_32         0x80000000         //      DEC 2147483648   BIN:  1000 0000 0000 0000 0000 0000 0000 0000
#define  BIT_31         0x40000000         //      DEC 1073741824	BIN:  0100 0000 0000 0000 0000 0000 0000 0000
#define  BIT_30         0x20000000         //      DEC 536870912	BIN:  0010 0000 0000 0000 0000 0000 0000 0000
#define  BIT_29         0x10000000         //      DEC 268435456	BIN:  0001 0000 0000 0000 0000 0000 0000 0000
#define  BIT_28         0x8000000          //      DEC 134217728	BIN:  0000 1000 0000 0000 0000 0000 0000 0000
#define  BIT_27         0x4000000          //      DEC 67108864	    BIN:  0000 0100 0000 0000 0000 0000 0000 0000
#define  BIT_26         0x2000000          //      DEC 33554432	    BIN:  0000 0010 0000 0000 0000 0000 0000 0000
#define  BIT_25         0x1000000          //      DEC 16777216	    BIN:  0000 0001 0000 0000 0000 0000 0000 0000
#define  BIT_24         0x800000           //      DEC 8388608		BIN:  0000 0000 1000 0000 0000 0000 0000 0000
#define  BIT_23         0x400000           //      DEC 4194304		BIN:  0000 0000 0100 0000 0000 0000 0000 0000
#define  BIT_22         0x200000           //      DEC 2097152		BIN:  0000 0000 0010 0000 0000 0000 0000 0000
#define  BIT_21         0x100000           //      DEC 1048576		BIN:  0000 0000 0001 0000 0000 0000 0000 0000
#define  BIT_20         0x80000            //      DEC 524288		BIN:  0000 0000 0000 1000 0000 0000 0000 0000
#define  BIT_19         0x40000            //      DEC 262144		BIN:  0000 0000 0000 0100 0000 0000 0000 0000
#define  BIT_18         0x20000            //      DEC 131072		BIN:  0000 0000 0000 0010 0000 0000 0000 0000
#define  BIT_17         0x10000	           //      DEC 65536		BIN:  0000 0000 0000 0001 0000 0000 0000 0000
#define  BIT_16         0x8000             //      DEC 32768        BIN:  0000 0000 0000 0000 1000 0000 0000 0000
#define  BIT_15         0x4000             //      DEC 16384        BIN:  0000 0000 0000 0000 0100 0000 0000 0000
#define  BIT_14         0x2000             //      DEC 8192         BIN:  0000 0000 0000 0000 0010 0000 0000 0000
#define  BIT_13         0x1000             //      DEC 4096         BIN:  0000 0000 0000 0000 0001 0000 0000 0000
#define  BIT_12         0x800              //      DEC 2048         BIN:  0000 0000 0000 0000 0000 1000 0000 0000
#define  BIT_11         0x400              //      DEC 1024         BIN:  0000 0000 0000 0000 0000 0100 0000 0000
#define  BIT_10         0x200              //      DEC 512          BIN:  0000 0000 0000 0000 0000 0010 0000 0000
#define  BIT_9          0x100              //      DEC 256          BIN:  0000 0000 0000 0000 0000 0001 0000 0000
#define  BIT_8          0x80               //      DEC 128          BIN:  0000 0000 0000 0000 0000 0000 1000 0000
#define  BIT_7          0x40               //      DEC 64           BIN:  0000 0000 0000 0000 0000 0000 0100 0000
#define  BIT_6          0x20               //      DEC 32           BIN:  0000 0000 0000 0000 0000 0000 0010 0000
#define  BIT_5          0x10               //      DEC 16           BIN:  0000 0000 0000 0000 0000 0000 0001 0000
#define  BIT_4          0x8                //      DEC 8            BIN:  0000 0000 0000 0000 0000 0000 0000 1000
#define  BIT_3          0x4                //      DEC 4            BIN:  0000 0000 0000 0000 0000 0000 0000 0100
#define  BIT_2          0x2                //      DEC 2            BIN:  0000 0000 0000 0000 0000 0000 0000 0010
#define  BIT_1          0x1                //      DEC 1            BIN:  0000 0000 0000 0000 0000 0000 0000 0001


/// THIS ALL NEEDS TO BE FIXED, NO REASON FOR TEMPLATES WHEN THE INPUT TYPE IS ALMOST ALWAYS THE SAME
template<typename T>
T ConcatenateBitsets(T _highbits, T _lowbits)
{// Combines Two values bits to generate a new value
	return ((int_fast64_t)_highbits << 32) | ((int_fast64_t)_lowbits & MAX_INTEGER);
}


// USE CASE: New_Value = My_Value & Mask(X)
template<typename T>
T BitMask(T _value)
{ // Creates a Bitmask from Value
	return  BIT(_value) - 1;
}

// USE CASE: int32_t New_Value = SetBit(8, 0xFFFFFFFF);                           [ RESULTS: FFFFFF7F ]
template<typename T>
T SetBit(T _value, int bitIndex, bool _state)
{// Sets the Value of a Specific Bit for a given Value.
	if (_state == true)
	{
		return _value | BIT(bitIndex);
	}
	return _value & ~BIT(bitIndex);
}

// USE CASE: int32_t New_Value = ExtractLowBits<int32_t>(16, int32_t 0xFFFFFFFF); [ RESULTS:0x0000FFFF ]
template<typename T>
T ExtractBitRange(T _value, unsigned char from, unsigned char to)
{ // Extracts the result of the bits to-from in T _value and returns results
	unsigned mask = (BIT((to - from + 1)) - 1) << from;
	return (_value & mask) >> from;
}

// USE CASE: int32_t New_Value = ExtractLowBitRange(16, 0xFFFFFFFF);              [ RESULTS: 0xFFFF ]
template<typename T>
T ExtractLowBitRange(char bitcount, T _value)
{ // Extracts the Bottom bits from T Value for Bitcount number of Bits and returns the results 
	return _value & (BIT(bitcount) - 1);
}

// USE CASE: int32_t New_Value = ExtractHigh16Bits(0xFFF7FFFF)                    [RESULTS: FFF7]
template<typename T>
T ExtractHigh16Bits(T _value)
{// Extracts the Top 16 bits from Likely a 32bit word.
	return  (_value & (0x1FFFF << (32 - 17))) >> (32 - 17);
}


#define MYSTIC_KEY_PRESS               GLFW_PRESS
#define MYSTIC_KEY_RELEASE             GLFW_RELEASE
#define MYSTIC_KEY_REPEAT              GLFW_REPEAT
 
#define NO_EVENT                  0x00000000

#define EVENT_SYSTEM              (BIT_32)
#define EVENT_TIMER			      (EVENT_SYSTEM | BIT_1)   
#define EVENT_CLIPBOARD		      (EVENT_SYSTEM | BIT_2)   
#define EVENT_DRAGDROP            (EVENT_SYSTEM | BIT_3)   // void drop_callback(GLFWwindow* window, int count, const char** paths)
#define EVENT_ERROR               (EVENT_SYSTEM | BIT_4)   // void error_callback(int error, const char* description)

#define EVENT_TEXTINPUT           (BIT_31)                    // void character_callback(GLFWwindow* window, unsigned int codepoint)
#define EVENT_MODTEXTINPUT        (EVENT_TEXTINPUT | BIT_1)   // void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods)

#define EVENT_MOUSE               (BIT_30)  
#define EVENT_MOUSEBUTTON         (EVENT_MOUSE | BIT_1)  // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
#define EVENT_MOUSEMOVE           (EVENT_MOUSE | BIT_2)  // static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)

#define EVENT_MOUSEDOWN_LEFT      (EVENT_MOUSEBUTTON | BIT_5)   // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
#define EVENT_MOUSEDOWN_RIGHT     (EVENT_MOUSEBUTTON | BIT_6)   // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
#define EVENT_MOUSEDOWN_MIDDLE    (EVENT_MOUSEBUTTON | BIT_7)   // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)

#define EVENT_MOUSEUP_LEFT        (EVENT_MOUSEBUTTON | BIT_9)   // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
#define EVENT_MOUSEUP_RIGHT       (EVENT_MOUSEBUTTON | BIT_10)  // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
#define EVENT_MOUSEUP_MIDDLE      (EVENT_MOUSEBUTTON | BIT_11)  // void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)

#define EVENT_KEYBOARD            (BIT_29)// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
#define EVENT_KEYDOWN             (EVENT_KEYBOARD | BIT_1)  
#define EVENT_KEYUP               (EVENT_KEYBOARD | BIT_2)  
#define EVENT_KEYREPEAT           (EVENT_KEYBOARD | BIT_3)
#define EVENT_KEYUNKNOWN          (EVENT_KEYBOARD | BIT_4)

#define EVENT_WINDOW              (BIT_28)
#define EVENT_WINDOWSCROLL        (EVENT_WINDOW | BIT_1)   // void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
#define EVENT_WINDOW_ENTER        (EVENT_WINDOW | BIT_2)   // void cursor_enter_callback(GLFWwindow* window, int entered)
#define EVENT_WINDOW_RESIZE       (EVENT_WINDOW | BIT_3)
#define EVENT_WINDOW_CLOSE        (EVENT_WINDOW | BIT_4)
#define EVENT_WINDOW_MOVE         (EVENT_WINDOW | BIT_5)
#define EVENT_WINDOW_MINIMIZE     (EVENT_WINDOW | BIT_6)
 
#define EVENT_JOYSTICK            (BIT_27)   // void joystick_callback(int joy, int event)


#define MSG_MAX_RANGE          0xFFFFFFFF
#define MSG_KEY_PRESS          GLFW_PRESS
#define MSG_KEY_REPEAT         GLFW_REPEAT
#define MSG_KEY_RELEASE        GLFW_RELEASE
#define MSG_KEY_UNKNOWN        GLFW_KEY_UNKNOWN

#define MOUSE_BUTTONDOWN            1
#define MOUSE_BUTTONUP              0

#define MOUSE_BUTTON_LEFT     GLFW_MOUSE_BUTTON_1   
#define MOUSE_BUTTON_RIGHT    GLFW_MOUSE_BUTTON_2   
#define MOUSE_BUTTON_CENTER   GLFW_MOUSE_BUTTON_3   

// RETURN JUST THE UPPER 8 BITS TO BE MASKED WITH THE MSG_GROUP TYPE TO RETURN TRUE OR FALSE
#define GET_CATEGORY(x)    (x >> 20) 
#define MESSAGE_MASK       0x00FFFFFF


// MESSAGE HANDLING METHOD NULL is DEFAULT KEEP
// REMOVE FROM QUEUE, KEEP IN QUEUE OR REPOST TO THE FRONT OF THE QUEUE
#define KEEP_MESSAGE                0x00000000
#define REMOVE_MESSAGE              0x000000F0
#define REPOST_MESSAGE              0x00000F00


// ===================================================================================================================================================
//                                     KEY REDEFINES
// ===================================================================================================================================================

#define MYSTIC_KEY_UNKNOWN             GLFW_KEY_UNKNOWN        
#define MYSTIC_KEY_SPACE			   GLFW_KEY_SPACE          
#define MYSTIC_KEY_APOSTROPHE		   GLFW_KEY_APOSTROPHE     
#define MYSTIC_KEY_COMMA			   GLFW_KEY_COMMA          
#define MYSTIC_KEY_MINUS			   GLFW_KEY_MINUS          
#define MYSTIC_KEY_PERIOD			   GLFW_KEY_PERIOD         
#define MYSTIC_KEY_SLASH			   GLFW_KEY_SLASH          
#define MYSTIC_KEY_0				   GLFW_KEY_0              
#define MYSTIC_KEY_1				   GLFW_KEY_1              
#define MYSTIC_KEY_2				   GLFW_KEY_2              
#define MYSTIC_KEY_3				   GLFW_KEY_3              
#define MYSTIC_KEY_4				   GLFW_KEY_4              
#define MYSTIC_KEY_5				   GLFW_KEY_5              
#define MYSTIC_KEY_6				   GLFW_KEY_6              
#define MYSTIC_KEY_7				   GLFW_KEY_7              
#define MYSTIC_KEY_8				   GLFW_KEY_8              
#define MYSTIC_KEY_9				   GLFW_KEY_9              
#define MYSTIC_KEY_SEMICOLON		   GLFW_KEY_SEMICOLON      
#define MYSTIC_KEY_EQUAL			   GLFW_KEY_EQUAL          
#define MYSTIC_KEY_A				   GLFW_KEY_A              
#define MYSTIC_KEY_B				   GLFW_KEY_B              
#define MYSTIC_KEY_C				   GLFW_KEY_C              
#define MYSTIC_KEY_D				   GLFW_KEY_D              
#define MYSTIC_KEY_E				   GLFW_KEY_E              
#define MYSTIC_KEY_F				   GLFW_KEY_F              
#define MYSTIC_KEY_G				   GLFW_KEY_G              
#define MYSTIC_KEY_H				   GLFW_KEY_H              
#define MYSTIC_KEY_I				   GLFW_KEY_I              
#define MYSTIC_KEY_J				   GLFW_KEY_J              
#define MYSTIC_KEY_K				   GLFW_KEY_K              
#define MYSTIC_KEY_L				   GLFW_KEY_L              
#define MYSTIC_KEY_M				   GLFW_KEY_M              
#define MYSTIC_KEY_N				   GLFW_KEY_N              
#define MYSTIC_KEY_O				   GLFW_KEY_O              
#define MYSTIC_KEY_P				   GLFW_KEY_P              
#define MYSTIC_KEY_Q				   GLFW_KEY_Q              
#define MYSTIC_KEY_R				   GLFW_KEY_R              
#define MYSTIC_KEY_S				   GLFW_KEY_S              
#define MYSTIC_KEY_T				   GLFW_KEY_T              
#define MYSTIC_KEY_U				   GLFW_KEY_U              
#define MYSTIC_KEY_V				   GLFW_KEY_V              
#define MYSTIC_KEY_W				   GLFW_KEY_W              
#define MYSTIC_KEY_X				   GLFW_KEY_X              
#define MYSTIC_KEY_Y				   GLFW_KEY_Y              
#define MYSTIC_KEY_Z				   GLFW_KEY_Z              
#define MYSTIC_KEY_LEFT_BRACKET		   GLFW_KEY_LEFT_BRACKET   
#define MYSTIC_KEY_BACKSLASH		   GLFW_KEY_BACKSLASH      
#define MYSTIC_KEY_RIGHT_BRACKET	   GLFW_KEY_RIGHT_BRACKET  
#define MYSTIC_KEY_GRAVE_ACCENT		   GLFW_KEY_GRAVE_ACCENT   
#define MYSTIC_KEY_WORLD_1			   GLFW_KEY_WORLD_1        
#define MYSTIC_KEY_WORLD_2			   GLFW_KEY_WORLD_2        
#define MYSTIC_KEY_ESCAPE			   GLFW_KEY_ESCAPE         
#define MYSTIC_KEY_ENTER			   GLFW_KEY_ENTER          
#define MYSTIC_KEY_TAB				   GLFW_KEY_TAB            
#define MYSTIC_KEY_BACKSPACE		   GLFW_KEY_BACKSPACE      
#define MYSTIC_KEY_INSERT			   GLFW_KEY_INSERT         
#define MYSTIC_KEY_DELETE			   GLFW_KEY_DELETE         
#define MYSTIC_KEY_RIGHT			   GLFW_KEY_RIGHT          
#define MYSTIC_KEY_LEFT				   GLFW_KEY_LEFT           
#define MYSTIC_KEY_DOWN				   GLFW_KEY_DOWN           
#define MYSTIC_KEY_UP				   GLFW_KEY_UP             
#define MYSTIC_KEY_PAGE_UP			   GLFW_KEY_PAGE_UP        
#define MYSTIC_KEY_PAGE_DOWN		   GLFW_KEY_PAGE_DOWN      
#define MYSTIC_KEY_HOME				   GLFW_KEY_HOME           
#define MYSTIC_KEY_END				   GLFW_KEY_END            
#define MYSTIC_KEY_CAPS_LOCK		   GLFW_KEY_CAPS_LOCK      
#define MYSTIC_KEY_SCROLL_LOCK		   GLFW_KEY_SCROLL_LOCK    
#define MYSTIC_KEY_NUM_LOCK			   GLFW_KEY_NUM_LOCK       
#define MYSTIC_KEY_PRINT_SCREEN		   GLFW_KEY_PRINT_SCREEN   
#define MYSTIC_KEY_PAUSE			   GLFW_KEY_PAUSE          
#define MYSTIC_KEY_F1				   GLFW_KEY_F1             
#define MYSTIC_KEY_F2				   GLFW_KEY_F2             
#define MYSTIC_KEY_F3				   GLFW_KEY_F3             
#define MYSTIC_KEY_F4				   GLFW_KEY_F4             
#define MYSTIC_KEY_F5				   GLFW_KEY_F5             
#define MYSTIC_KEY_F6				   GLFW_KEY_F6             
#define MYSTIC_KEY_F7				   GLFW_KEY_F7             
#define MYSTIC_KEY_F8				   GLFW_KEY_F8             
#define MYSTIC_KEY_F9				   GLFW_KEY_F9             
#define MYSTIC_KEY_F10				   GLFW_KEY_F10            
#define MYSTIC_KEY_F11				   GLFW_KEY_F11            
#define MYSTIC_KEY_F12				   GLFW_KEY_F12            
#define MYSTIC_KEY_F13				   GLFW_KEY_F13            
#define MYSTIC_KEY_F14				   GLFW_KEY_F14            
#define MYSTIC_KEY_F15				   GLFW_KEY_F15            
#define MYSTIC_KEY_F16				   GLFW_KEY_F16            
#define MYSTIC_KEY_F17				   GLFW_KEY_F17            
#define MYSTIC_KEY_F18				   GLFW_KEY_F18            
#define MYSTIC_KEY_F19				   GLFW_KEY_F19            
#define MYSTIC_KEY_F20				   GLFW_KEY_F20            
#define MYSTIC_KEY_F21				   GLFW_KEY_F21            
#define MYSTIC_KEY_F22				   GLFW_KEY_F22            
#define MYSTIC_KEY_F23				   GLFW_KEY_F23            
#define MYSTIC_KEY_F24				   GLFW_KEY_F24            
#define MYSTIC_KEY_F25				   GLFW_KEY_F25            
#define MYSTIC_KEY_KP_0				   GLFW_KEY_KP_0           
#define MYSTIC_KEY_KP_1				   GLFW_KEY_KP_1           
#define MYSTIC_KEY_KP_2				   GLFW_KEY_KP_2           
#define MYSTIC_KEY_KP_3				   GLFW_KEY_KP_3           
#define MYSTIC_KEY_KP_4				   GLFW_KEY_KP_4           
#define MYSTIC_KEY_KP_5				   GLFW_KEY_KP_5           
#define MYSTIC_KEY_KP_6				   GLFW_KEY_KP_6           
#define MYSTIC_KEY_KP_7				   GLFW_KEY_KP_7           
#define MYSTIC_KEY_KP_8				   GLFW_KEY_KP_8           
#define MYSTIC_KEY_KP_9				   GLFW_KEY_KP_9           
#define MYSTIC_KEY_KP_DECIMAL		   GLFW_KEY_KP_DECIMAL     
#define MYSTIC_KEY_KP_DIVIDE		   GLFW_KEY_KP_DIVIDE      
#define MYSTIC_KEY_KP_MULTIPLY		   GLFW_KEY_KP_MULTIPLY    
#define MYSTIC_KEY_KP_SUBTRACT		   GLFW_KEY_KP_SUBTRACT    
#define MYSTIC_KEY_KP_ADD			   GLFW_KEY_KP_ADD         
#define MYSTIC_KEY_KP_ENTER			   GLFW_KEY_KP_ENTER       
#define MYSTIC_KEY_KP_EQUAL			   GLFW_KEY_KP_EQUAL       
#define MYSTIC_KEY_LEFT_SHIFT		   GLFW_KEY_LEFT_SHIFT     
#define MYSTIC_KEY_LEFT_CONTROL		   GLFW_KEY_LEFT_CONTROL   
#define MYSTIC_KEY_LEFT_ALT			   GLFW_KEY_LEFT_ALT       
#define MYSTIC_KEY_LEFT_SUPER		   GLFW_KEY_LEFT_SUPER     
#define MYSTIC_KEY_RIGHT_SHIFT		   GLFW_KEY_RIGHT_SHIFT    
#define MYSTIC_KEY_RIGHT_CONTROL	   GLFW_KEY_RIGHT_CONTROL  
#define MYSTIC_KEY_RIGHT_ALT		   GLFW_KEY_RIGHT_ALT      
#define MYSTIC_KEY_RIGHT_SUPER		   GLFW_KEY_RIGHT_SUPER    
#define MYSTIC_KEY_MENU				   GLFW_KEY_MENU           
#define MYSTIC_KEY_LAST				   GLFW_KEY_LAST 

#endif

bool GLLogCall(const char *function, const char *file, int line);
void GLCheckError();
void GLClearError();

std::ostream& operator <<(std::ostream& S, Mat4 _other);





Vec3 ExtractPosition(Mat4 _param);



/*

Begin/End
Create/Destroy
Open/Close
Next/Previous
Put/Get
Sources/Destination

*/