/*
*                         Vortex OpenSplice
*
 *   This software and documentation are Copyright 2006 to  ADLINK
 *   Technology Limited, its affiliated companies and licensors. All rights
 *   reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
*
*/


/**
 * @file
 */

#include <org/opensplice/pub/CoherentSetDelegate.hpp>
#include <org/opensplice/pub/PublisherDelegate.hpp>


namespace org
{
namespace opensplice
{
namespace pub
{

CoherentSetDelegate::CoherentSetDelegate(const dds::pub::Publisher& pub) : pub(pub), ended(false)
{
    ISOCPP_REPORT_STACK_DDS_BEGIN(pub);

    pub.delegate()->begin_coherent_changes();
}

CoherentSetDelegate::~CoherentSetDelegate()
{
    /* The wrapper destructor already ended the set. Nothing to do here. */
}

void
CoherentSetDelegate::end()
{
    if(!ended)
    {
        pub.delegate()->end_coherent_changes();
        ended = true;
    }
}

bool
CoherentSetDelegate::operator ==(const CoherentSetDelegate& other) const
{
    return pub == other.pub && ended == other.ended;
}

}
}
}
