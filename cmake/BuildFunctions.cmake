

################################################################################
# FUNCTION project_group
################################################################################
function(project_group target name)
    set_target_properties(${target} PROPERTIES FOLDER ${name})
endfunction(project_group)


################################################################################
