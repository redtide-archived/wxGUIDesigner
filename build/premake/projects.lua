-----------------------------------------------------------------------------
-- Name:        projects.lua
-- Purpose:     Projects support script.
-- Author:      Andrea Zanellato
-- Modified by:
-- Created:     2011/11/21
-- Revision:    $Hash$
-- Copyleft:    (ɔ) Andrea Zanellato
-- Licence:     GNU General Public License Version 3
-----------------------------------------------------------------------------
CustomPrefix = "wxgd"
-- dofile( scriptDir .. "/lua.lua" )
-- dofile( scriptDir .. "/codegenerator.lua" )
if _ACTION ~= "gmake" then
    dofile( scriptDir .. "/boost.lua" )
    dofile( scriptDir .. "/buildscripts.lua" )
    dofile( scriptDir .. "/doxygen.lua" )
    dofile( scriptDir .. "/interfaces.lua" )
end
    dofile( scriptDir .. "/core.lua" )
    dofile( scriptDir .. "/wxguidesigner.lua" )
    dofile( scriptDir .. "/utilities.lua" )
