// Copyright [2015] Ruud Cools

#ifndef MATH_OTSA_H_
#define MATH_OTSA_H_

#include "./TSA.h"  // the tsa

namespace tsa {
    /**
     * Namespace which defines all algorithms which could deterine ordering of traces
     * This namespace is part of the tsa namespace
     */
    namespace otsa {
        /**
         * Given TraceSections create an order for which:
         * Each sections which is inside another is ordered earlier then the including section
         * Sections which are mutual include are ordered such that the distance between them is maximised
         * @param[in/out] i_traces TraceSections for are ordered such that:
         * - the first includes at least all others
         * - all sections except the last are closed sections
         * @param[in] i_position The start position which the sections will be orded to.
         * @param[in/out] io_vector vector to which the orded i_traces will be added.
         */
        void orderVector (TraceSection* i_traces,
                          const Point2D& i_position);

    /**
     * Append a section to the io_vector given traces in the i_section.
     * If i_section is closed, find the nearest traces and continue from there,
     * If i_section is open, find the furthust trace and keep on finding furthest sections
     */
    void appendSection(TraceSection i_section,
                       Point2D* i_virtualPosition,
                       TraceSection* io_vector);

    /**
     * Given TraceSections add the largest section and its included sections
     * @param[in/out] o_sections Sections from which the largest is
     * found and other sections which are inside this section
     * @param[in/out] o_position This position is used to order the sections and indicates the new position
     * @param[in/out] o_ordedVector The resulting vector in which all traces are nicely orded
     */
    TraceSection handleLargestSection(TraceSections* o_sections,
                                      Point2D* o_position,
                                      TraceSection* o_ordedVector);

    /**
     * Append the appropiate traces to the orded vector.
     * @param[in/out] o_section TraceSection all isolated traces inside this section will be added
     * @param[in/out] o_position Position which determines the order of the isolated traces
     * @param[in/out] o_isolatedTraces A group of individual traces from which appropiate traces will be added
     * @param[in/out] o_orderedvector The resulting vector in which all traces are nicely orded
     */
    void handleIsolatedTraces(const TraceSection& o_section,
                              Point2D* o_position,
                              TraceSection* o_isolatedTraces,
                              TraceSection* o_ordedVector);

    /**
     * Given a number of independend sections append them to the ordered vector
     * in such a way heat will be the least for each section
     * @param[in] i_independendSections Group of sections which are not dependend on each other,
     * So essentially, the order they will be done does have no effect on the sections them selfs
     * @param[in/out] io_orderedTracepointervector Vector which has the ordered traces
     * @param[in/out] The position which is used to determine which sectons is cut first
     */
    void handleIndependendSections(TraceSections i_independedSections,
                                   TraceSection* io_orderedTracePointerVector,
                                   Point2D* i_virtualPosition);
    }
}
#endif  // MATH_OTSA_H_
