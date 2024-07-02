// // Remove the newline character from the end of the line
        // if(current_line.find("\n") == current_line.length() - 1){
        //     current_line = current_line.substr(0, current_line.length() - 1);
        // }
       
        // //If there is a comment in the line, remove the comment
        // size_t comment_pos = current_line.find("#");
        // if(comment_pos != std::string::npos){
        //     current_line = current_line.substr(0, comment_pos);
        // }

        // if(std::all_of(current_line.begin(), current_line.end(), isspace)){
        //     std::cout << "Empty line" << std::endl;
        //     continue;
        // }


        // //current_line = removeWhitespace(current_line);

        // // Analyze the token, 
        // std::vector<Token> analyzed_tokens = analyze(current_line, line_number);
        // if(analyzed_tokens.size() == 1 && analyzed_tokens[0].get_type() == TokenType::ERROR_TOKEN){ //analyze returns a single error token if there is an error
        //     std::cout << "Error: Invalid token " << current_line << " on line " << line_number << std::endl;
        //     return std::vector<Token>();
        // }

        // for(Token token : analyzed_tokens){
        //     tokens.push_back(token);
        // }

        // line_number++;