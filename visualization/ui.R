library(shiny)

# Define UI for application that draws a histogram
shinyUI(fluidPage(

  # Application title
  titlePanel("DABE"),

  mainPanel(
    tabsetPanel(type = "tabs",id = "tabs", 
      tabPanel("Profiles", value ="Profiles",
        plotOutput("distPlot"),
        hr(),
        fluidRow(
            fileInput("file1","Choose a File")
            )
        )
    )
)

))

