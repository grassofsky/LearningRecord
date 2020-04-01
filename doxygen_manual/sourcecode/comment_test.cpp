/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *  
 * etc...
 */

/// \brief brief description is start with %\brief
/// 
/// This is a detail description
void test0() {}

///////////////////////////////
/// \brief brief description is start with %\brief
/// 
/// This is a detail description
///////////////////////////////
void test1() {}

 /*! 
  *  A list of events:
  *    - mouse events
  *         -# mouse move event
  *         -# mouse click event\n
  *            More info about the click event.
  *         -# mouse double click event
  *
  *      End of first list
  *
  *    - keyboard events
  *         1. key down event
  *         2. key up event
  *
  *  More text here.
  */
void test2() {}

/*!
 * \brief brief description
 *
 * This is a detail description
 */
void test3() {}

/*!
\brief brief description

 This is a detail description
*/
void test4() {}

class Test 
{
public:
    /** @name Group2
    *  Description of group 2. 
    */
    ///@{
    /** Function 2 in group 2. Details. */
    void func2InGroup2() {}
    /** Function 1 in group 2. Details. */
    void func1InGroup2() {}
    ///@}

    int value0; //!< member description
    int value1; /*!< member description */
    int value2; ///< member description
    int vluae3; // This is not be parsed
};


/// \addtogroup A
/// @{
///
int bool InA;
/// @}

/**
 * \ingroup A
 */
extern int VarInA;

/**
 * \defgroup IntVariables Global integer variables
 * @{
 */

/** an integer variable */
extern int IntegerVariable;

/**@}*/

/**
 * \defgroup Variables Global variables
 */
/**@{*/

/** a variable in group A */
int VarInA;

int IntegerVariable;

/**@}*/

