//
//  wim_doc_template.h
//  EE264_lab
//
//  Created by Fernando Mujica on 4/8/19.
//  Copyright © 2019 Fernando Mujica. All rights reserved.
//

#ifndef wim_doc_template_h
#define wim_doc_template_h


/*! \mainpage wim_doc_template.h
 *
 * \section intro_sec Introduction
 *
 * Intro lab for EE264 WiM option
 *
 * \section description_sec Description
 *
 * - This file is a code documentation template using Doxygen syntax
 *
 * \section elements_sec Elements
 *
 * \subsection lists_subsec Numbered lists
 *
 * 1. First item.
 * 2. Second item.
 *
 * \subsection tables_subsec Tables
 *
 *  First Header  | Second Header
 *  ------------- | -------------
 *  Content Cell  | Content Cell
 *  Content Cell  | Content Cell *
 *
 * \subsection code_sec Embedding code
 *
 * This a normal paragraph
 *
 *     This is a code block
 *
 * We continue with a normal paragraph again.
 *
 * \section equation_subsec Equations
 *
 * The distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is
 * \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
 *
 * \f[
 *  |I_2|=\left| \int_{0}^T \psi(t)
 *            \left\{
 *              u(a,t)-
 *                \int_{\gamma(t)}^a
 *                \frac{d\theta}{k(\theta,t)}
 *                \int_{a}^\theta c(\xi)u_t(\xi,t)\,d\xi
 *             \right\} dt
 *          \right|
 * \f]
 *
 * \f{eqnarray*}{
 *         g &=& \frac{Gm_2}{r^2} \\
 *           &=& \frac{(6.673 \times 10^{-11}\,\mbox{m}^3\,\mbox{kg}^{-1}\,
 *               \mbox{s}^{-2})(5.9736 \times 10^{24}\,\mbox{kg})}{(6371.01\,\mbox{km})^2} \\
 *           &=& 9.82066032\,\mbox{m/s}^2
 * \f}
 *
 * \section image_sec Including images
 *
 * The sampling rate conversion application system block diagram is shown below:
 *  \image html  ee264_lab.jpg "DSP System"
 *  \image latex ee264_lab.eps "DSP System" width=10cm
 *
 * \section conclusion_sec Conclusion
 *
 * For more information on Doxygen syntax see [the documentation](http://doxygen.org).
 */


/*! The setup function runs once at the begining of the program.
 *
 *  Use this function to initialize variables, allocate memory, etc.
 */
void wim_setup();

/*! Audio callback function
 *
 *  The processAudio() function is declared as an extern function inside the Audio.cpp
 *  file of the Audio library.
 *
 *  The Audio library creates an instance of the AudioClass, named AudioC, that is accesible
 *  by processAudio.  The AudioClass in turns defines arrays for the input and output buffers.
 *  The buffers are of type int (16 bits in the C55x family of processors).
 *
 *  AudioC.inputLeft   = Left input channel buffer
 *  AudioC.inputRight  = Right input channel buffer
 *  AudioC.outputLeft  = Left output channel buffer
 *  AudioC.outputRight = Right output channel buffer
 *
 *  The processAudio() fucntion is called by the interrupt service routine (ISR)
 *  once a full block of data is received from the audio codec digital interface and available in
 *  the input buffers.
 *  After the audioProcess() function executes to completion, data on the output buffers is sent
 *  to the codec audio output.
 */
void wim_processAudio();

#endif /* wim_doc_template_h */
