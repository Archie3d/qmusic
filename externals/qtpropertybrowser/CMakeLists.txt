# Copyright (c) 2014 Biocartis N.V.,
# All Rights Reserved.
#
# This source code and any compilation or derivative thereof is the
# proprietary information of Biocartis N.V. and is confidential in nature.
# Under no circumstances is this software to be combined with any Open Source
# Software in any way or placed under an Open Source License of any type without
# the express written permission of Biocartis N.V.
#
#               Project: Idylla Studio
#            File %name: CMakeLists.txt %
#              %version: 2 %
#        %date_modified: %
#           %derived_by: abenilov %
#         %date_created: Wed Jan  7 17:54:10 2015 %
#           %created_by: abenilov %
#
#  CM Synergy %full_filespec: CMakeLists.txt~2:ascii:blr01apo#7 %
#

project(qtpropertybrowser)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)

include(build_shared_library)