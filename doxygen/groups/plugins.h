///////////////////////////////////////////////////////////////////////////////
// Name:        plugins.h
// Purpose:     Plugin description page of the Doxygen manual
// Author:      Andrea Zanellato
// Modified by: 
// Created:     2011/12/13
// Revision:    $Hash$
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
@defgroup group_plugins Plugins

@section page_plugin_code XML plugin

In wxGUIDesigner all classes are defined in a XML file

@htmlonly
    <form>
        <textarea id="code" name="code">
&lt;?xml version="1.0" encoding="UTF-8"?&gt;
&lt;!-- wxFrame --&gt;
&lt;class name="wxFrame"&gt;
    &lt;include&gt;classes/toplevel.xml&lt;/include&gt;
    &lt;inherits&gt;
        &lt;class name="wxTopLevelWindow"/&gt;
    &lt;/inherits&gt;
    &lt;properties&gt;
        &lt;name&gt;m_frame&lt;/name&gt;
        &lt;styles name="Styles"&gt;
            &lt;style name="wxDEFAULT_FRAME_STYLE"&gt;
                &lt;description&gt;Defined as wxMINIMIZE_BOX...&lt;/description&gt;
            &lt;/style&gt;
            ...
        &lt;/styles&gt;
        &lt;styles name="Extra styles"&gt;
            &lt;style name="wxFRAME_EX_CONTEXTHELP"&gt;
                &lt;description&gt;Under Windows, puts a query button...&lt;/description&gt;
            &lt;/style&gt;
            ...
        &lt;/styles&gt;
        ...
    &lt;/properties&gt;
    &lt;events&gt;
        &lt;class name="wxCloseEvent"&gt;
            &lt;event name="EVT_CLOSE"&gt;
                &lt;description&gt;The frame is being closed by ...&lt;/description&gt;
            &lt;/event&gt;
        &lt;/class&gt;
        ...
    &lt;/events&gt;
&lt;/class&gt;
        </textarea>
    </form>
    <script>
        var editor = CodeMirror.fromTextArea(document.getElementById("code"),
        {
            mode:
            {
                name: "xml",
                alignCDATA: true
            },
            readOnly: true
        });
    </script>
@endhtmlonly

*/
