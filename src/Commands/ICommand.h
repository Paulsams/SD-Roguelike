#pragma once

struct ICommand
{
    bool canExecute();
    void execute();
};
