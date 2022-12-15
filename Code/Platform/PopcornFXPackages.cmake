# Rewrite the functions from cmake/3rdPartyPackages.cmake to be able to download from https://store.popcornfx.com
# This only handle the download part and let ly_force_download_package do the extracting part

function(pk_package_internal_download_package package_name package_id)
    unset(error_messages)

    ly_get_package_expected_hash(${package_name} package_expected_hash)

    set(server_url https://store.popcornfx.com)
    set(download_url ${server_url}/dl/get.php?id=${package_id})
    set(download_target ${LY_PACKAGE_DOWNLOAD_CACHE_LOCATION}/${package_name}${LY_PACKAGE_EXT})
    
    file(REMOVE ${download_target})

    ly_package_message(STATUS "ly_package: trying to download ${download_url} to ${download_target}")
    ly_get_package_expected_hash(${package_name} expected_package_hash)

    download_file(URL ${download_url} TARGET_FILE ${download_target} EXPECTED_HASH ${expected_package_hash} RESULTS results)
    list(GET results 0 status_code)
    
    if (${status_code} EQUAL 0 AND EXISTS ${download_target})
        ly_package_message(STATUS "ly_package:     - downloaded ${server_url} for package ${package_name}")
        return()
    else()
        # remove the status code and treat the rest of the list as the error.
        list(REMOVE_AT results 0)
        set(current_error_message "Error from server ${server_url} - ${status_code} - ${results}")
        #strip whitespace
        string(REGEX REPLACE "[ \t\r\n]$" "" current_error_message "${current_error_message}")
        list(APPEND error_messages "${current_error_message}")
        # we can't keep the file, sometimes it makes a zero-byte file!
        file(REMOVE ${download_target})
    endif()

    set(final_error_message "ly_package:     - Unable to get package ${package_name} from server ${server_url}.")
    foreach(error_message ${error_messages})
        set(final_error_message "${final_error_message}\n${error_message}")
    endforeach()
    ly_package_message(STATUS "${final_error_message}")
    #If we failed here, continue and let the ly-package-association try to download on LY_PACKAGE_SERVER_URLS
endfunction()

function(pk_force_download_package package_name package_id)
    ly_package_get_target_folder(${package_name} DOWNLOAD_LOCATION)

    # this function contains a REMOVE_RECURSE.  Because of that, we're going to do extra
    # validation on the inputs.
    # its not good enough for the variable to just exist but be empty, so we build strings
    if ("${package_name}" STREQUAL "" OR "${DOWNLOAD_LOCATION}" STREQUAL "")
        message(FATAL_ERROR "ly_package: ly_force_download_package called with invalid params!  Enable LY_PACKAGE_DEBUG to debug.")
    endif()

    set(final_folder ${DOWNLOAD_LOCATION}/${package_name})

    # is the package already present in the download cache, with the correct hash?
    set(temp_download_target ${LY_PACKAGE_DOWNLOAD_CACHE_LOCATION}/${package_name}${LY_PACKAGE_EXT})
    ly_get_package_expected_hash(${package_name} expected_package_hash)

    # can we reuse the download we already have in our download cache?
    if (EXISTS ${temp_download_target})
        ly_package_message(STATUS "The target ${temp_download_target} exists")
        file(SHA256 ${temp_download_target} existing_hash)
    endif()

    if (NOT "${existing_hash}" STREQUAL "${expected_package_hash}" )
        file(REMOVE ${temp_download_target})
        # we print this message unconditionally because downloading a package
        # can take time and we only get here if its missing in the first place, so 
        # this should happen once on the very first configure
        message(STATUS "Downloading package into ${final_folder}")
        ly_package_message(STATUS "ly_package:     - downloading package '${package_name}' to '${temp_download_target}'")
        pk_package_internal_download_package(${package_name} ${package_id})
    else()
        ly_package_message(STATUS "ly_package:     - package already correct hash ${temp_download_target}, re-using")
    endif()
endfunction()

function(pk_download_package_ifn package_name package_id)
    if ("${package_id}" STREQUAL "POPCORNFXPACKAGEIDTOREPLACE")
        return()
    endif()
    ly_validate_package(${package_name})
    if (NOT ${package_name}_VALIDATED)
        pk_force_download_package(${package_name} ${package_id})
    endif()
endfunction()