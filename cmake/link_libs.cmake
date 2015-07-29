# Link Qt libraries
if(USE_QT)
    qt5_use_modules(${PROJECT_NAME} Core ${DEPENDS_QT})    

    # Link Qt Property Browser
    if(USE_QT_PROPERTY_BROWSER)
        target_link_libraries(${PROJECT_NAME} ${QT_PROPERTY_BROWSER_LIB})
    endif()
    
endif()
